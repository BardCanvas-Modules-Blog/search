<?php
/**
 * Popular searches cloud for right sidebar
 * Called as include() from within a function
 *
 * @package    BardCanvas
 * @subpackage search
 * @author     Alejandro Caballero - lava.caballero@gmail.com
 *
 * Trailing vars:
 * @var module[]          $modules
 * @var module            $this_module    search
 * @var template          $template       The caller
 * @var \SimpleXMLElement $widget
 * @var string            $seed
 * @var string            $data_key
 * 
 * @var toolbox           $toolbox
 * 
 * @return string
 */

use hng2_base\module;
use hng2_base\template;
use hng2_modules\widgets_manager\toolbox;
use hng2_modules\search\search_history_repository;

global $config, $language, $mem_cache;

if( empty($config->globals["widgets_manager:toolbox"]) )
    $config->globals["widgets_manager:toolbox"] = new toolbox();
$toolbox = $config->globals["widgets_manager:toolbox"];
$placed_widget = $toolbox->get_widget_from_data_key($data_key);
if( is_null($placed_widget) ) return "";
$data = $toolbox->load_widget_data($placed_widget);

$repository = new search_history_repository();

$ttl = $data["cache_ttl"]; if( $ttl == "" ) $ttl = 1;
$ttl = $ttl * 60 * 60;
$res = null;

if( empty($data["cache_version"]) ) $data["cache_version"] = 1;
if($_GET["refresh_cache"] == "true") $data["cache_version"]++;

$all_terms = array();
$cache_key = "search.searches_cloud_widget_v{$data["cache_version"]}";
if($ttl > 0) $res = $mem_cache->get($cache_key);

if( ! is_null($res) )
{
    $all_terms = $res;
}
else
{
    $boundary  = date("Y-m-d", strtotime("today - 3 months"));
    $res = $repository->get_grouped_term_counts($boundary, $data["min_hits"]);
    foreach( $res as $key => $count ) $all_terms[$key] += $count;
    if($ttl > 0)
    {
        $mem_cache->set($cache_key, $all_terms, 0, $ttl);
        if($_GET["refresh_cache"] == "true")
        {
            $placed_widget->custom_data = $data;
            $toolbox->save_widget_data($placed_widget);
        }
    }
}

if( empty($all_terms) ) return "";

$tag_cloud = build_cloud($all_terms, 10, 12, 28);

ob_start(); ?>

<div class="tag_cloud">
    <? foreach($tag_cloud as $tag => $data):
        $title = htmlspecialchars(replace_escaped_vars(
            $this_module->language->search_hits,
            '{$hits}',
            $data->hits
        ));
        ?>
        <a style="font-size: <?= $data->size ?>px" href="<?= $config->full_root_path ?>/?s=<?= urlencode($tag) ?>"
           data-weight="<?= $data->weight ?>" title="<?= $title ?>"><?= $tag ?></a>
    <? endforeach; ?>
</div>

<? return ob_get_clean();
