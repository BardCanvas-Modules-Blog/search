<?php
/**
 * Compound index by hashtag
 *
 * @package    BardCanvas
 * @subpackage search
 * @author     Alejandro Caballero - lava.caballero@gmail.com
 *
 * @var config   $config
 * @var template $template
 * @var string   $tag
 * @var module   $current_module search
 */

use hng2_base\config;
use hng2_base\module;
use hng2_base\template;
use hng2_tools\record_browser;

$config->globals["search_by_tag_counts"]      = array();
$config->globals["search_by_tag_collections"] = array();
$current_module->load_extensions("search_by_tag", "collection_builder");

$highest_count = 0;
foreach($config->globals["search_by_tag_counts"] as $key => $count)
{
    if( $count > $highest_count ) $highest_count = $count;
    if( count($config->globals["search_by_tag_collections"][$key]) == 0 )
        unset( $config->globals["search_by_tag_collections"][$key] );
}

$limit  = 30;
$offset = (int) $_GET["offset"];

$browser    = new record_browser("search_by_tag_results");
$pagination = $browser->build_pagination($highest_count, $limit, $offset);
?>

<h1 class="clearfix">
    <span class="pull-right">
        <?= $pagination["this_page_number"] ?>
        /
        <?= $pagination["total_pages"] ?>
    </span>
    <?= replace_escaped_vars($current_module->language->pages->by_tag->title, '{$tag}', $tag) ?>
</h1>

<? if(count($config->globals["search_by_tag_collections"]) > 0 ): ?>
    <h2>
        <? foreach($config->globals["search_by_tag_collections"] as $title => $items): ?>
            <span class="framed_content inlined">
                <?= $title ?>: <?= number_format($config->globals["search_by_tag_counts"][$title]) ?>
            </span>
        <? endforeach; ?>
    </h2>
<? endif; ?>

<? if(count($config->globals["search_by_tag_collections"]) == 0): ?>
    <div class='framed_content state_highlight'>
        <span class='fa fa-info-circle'>
            <?= $current_module->language->pages->by_tag->no_records ?>
        </span>
    </div>
<? endif; ?>

<? include "{$template->abspath}/segments/search_by_tag_results.inc"; ?>

<div id="posts_pagination">
    <? $browser->render_pagination_links("{$config->full_root_path}/tag/{$tag}", $pagination); ?>
</div>
