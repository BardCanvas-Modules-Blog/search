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
 * @return string
 */

use hng2_base\module;
use hng2_base\template;
use hng2_modules\search\search_history_repository;

global $config, $language, $settings;

$repository = new search_history_repository();

$boundary  = date("Y-m-d H:i:s", strtotime("today - 6 months"));
$all_terms = array();

foreach( $repository->get_grouped_term_counts($boundary, $settings->get("modules:search.searches_cloud_min_hits"))
         as $key => $count ) $all_terms[$key] += $count;

if( empty($all_terms) ) return "";

$tag_cloud = build_cloud($all_terms);

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
