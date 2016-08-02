<?php
/**
 * Popular searches cloud for right sidebar
 * Called as include() from within a function
 *
 * @package    HNG2
 * @subpackage widgets_manager
 * @author     Alejandro Caballero - lava.caballero@gmail.com
 *
 * Trailing vars:
 * @var module[]          $modules
 * @var module            $this_module    widgets_manager
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

global $config, $language;

$repository = new search_history_repository();

$boundary  = date("Y-m-d H:i:s", strtotime("today - 6 months"));
$all_terms = array();

foreach( $repository->get_grouped_term_counts($boundary) as $key => $count )
    $all_terms[$key] += $count;

if( empty($all_terms) ) return "";

$tag_cloud = build_cloud($all_terms);

ob_start(); ?>

<div class="tag_cloud">
    <? foreach($tag_cloud as $tag => $data): ?>
        <a style="font-size: <?= $data->size ?>px" href="<?= $config->full_root_path ?>/tag/<?= $tag ?>"
           data-weight="<?= $data->weight ?>"><?= $tag ?></a>
    <? endforeach; ?>
</div>

<? return ob_get_clean();
