<?php
/**
 * Popular tags cloud for right sidebar
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
 * @return string
 */

use hng2_base\module;
use hng2_media\media_repository;
use hng2_base\template;
use hng2_modules\posts\posts_repository;

global $config, $language, $settings, $mem_cache;

$posts_repository = new posts_repository();
$media_repository = new media_repository();

$ttl = $settings->get("modules:search.tags_cloud_widget_cache_ttl"); if( $ttl == "" ) $ttl = 3;
$ttl = $ttl * 60 * 60;
$res = null;

$all_tags = array();
$cache_key = "search.tags_cloud_widget";
if($ttl > 0) $res = $mem_cache->get($cache_key);

if( ! is_null($res) )
{
    $all_tags = $res;
}
else
{
    $boundary = date("Y-m-d H:i:s", strtotime("today - 6 months"));
    
    foreach( $posts_repository->get_grouped_tag_counts($boundary, $settings->get("modules:search.tags_cloud_min_hits"))
             as $key => $count ) $all_tags[$key] += $count;
    
    foreach( $media_repository->get_grouped_tag_counts($boundary, $settings->get("modules:search.tags_cloud_min_hits"))
             as $key => $count ) $all_tags[$key] += $count;
    
    $config->globals["search_tags_cloud:boundary"] =& $boundary;
    $config->globals["search_tags_cloud:all_tags"] =& $all_tags;
    $this_module->load_extensions("tags_cloud_widget", "add_to_tag_counts");
    unset( $config->globals["search_tags_cloud:boundary"], $config->globals["search_tags_cloud:all_tags"] );
    
    if($ttl > 0) $mem_cache->set($cache_key, $all_tags, 0, $ttl);
}

if( empty($all_tags) ) return "";

$tag_cloud = build_cloud($all_tags);

ob_start(); ?>
    
    <div class="tag_cloud">
        <? foreach($tag_cloud as $tag => $data):
            $title = htmlspecialchars(replace_escaped_vars(
                $this_module->language->tag_hits,
                '{$hits}',
                $data->hits
            ));
            ?>
            <a style="font-size: <?= $data->size ?>px" href="<?= $config->full_root_path ?>/tag/<?= $tag ?>"
               data-weight="<?= $data->weight ?>" title="<?= $title ?>"><?= $tag ?></a>
        <? endforeach; ?>
    </div>

<? return ob_get_clean();
