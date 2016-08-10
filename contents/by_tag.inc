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

$config->globals["search_by_tag_collections"] = array();
$current_module->load_extensions("search_by_tag", "collection_builder");
?>

<h1 class="clearfix">
    <?= replace_escaped_vars($current_module->language->pages->by_tag->title, '{$tag}', $tag) ?>
</h1>

<? if(count($config->globals["search_by_tag_collections"]) == 0): ?>
    <div class='framed_content state_highlight'>
        <span class='fa fa-info-circle'>
            <?= $current_module->language->pages->by_tag->no_records ?>
        </span>
    </div>
<? endif; ?>

<? include "{$template->abspath}/segments/search_by_tag_results.inc"; ?>
