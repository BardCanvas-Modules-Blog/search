<?php
/**
 * Search results
 *
 * @package    BardCanvas
 * @subpackage search
 * @author     Alejandro Caballero - lava.caballero@gmail.com
 *
 * @var config            $config
 * @var \SimpleXMLElement $language
 * @var string            $config->globals["search_terms"]
 */

use hng2_base\config;
use hng2_modules\search\posts_repository_extender;
?>

<form class="search_form" method="get" action="<?= $config->full_root_path ?>/">
    <div class="form_wrapper">
        <input type="text" name="s" maxlength="100" 
               placeholder="<?= htmlspecialchars($language->search_placeholder) ?>"
               value="<?= htmlspecialchars(stripslashes($config->globals["search_terms"])) ?>">
        <span class="submit_icon fa fa-search fa-fw pseudo_link" onclick="$(this).closest('form').submit()"></span>
    </div>
</form>

<? if( empty($config->globals["search_terms"]) ): ?>
    
    <div class="framed_content state_ko">
        <span class="fa fa-warning"></span>
        <?= $current_module->language->messages->empty_search ?>
    </div>
    <? return; ?>
    
<? endif; ?>

<?
$repository = new posts_repository_extender();
$posts_data = $repository->search();

if( $posts_data->count == 0 )
{
    echo "
        <div class=\"framed_content state_highlight\">
            <span class=\"fa fa-info-circle\"></span>
            {$current_module->language->messages->empty_results}
        </div>
    ";
    
    return;
}
?>

<br>
<h1 class="clearfix">
    <?= replace_escaped_vars($current_module->language->messages->results, '{$count}', $posts_data->count) ?>
    <span class="pull-right"><?= $posts_data->pagination["this_page_number"]; ?>/<?= $posts_data->pagination["total_pages"]; ?></span>
</h1>

<?
# echo "<pre>{$repository->get_last_query()}</pre>";
$pagination_url_prefix = "$config->full_root_url/?s=" . urlencode($config->globals["search_terms"]);
$template->set("pagination_url_prefix", $pagination_url_prefix);
include "{$template->abspath}/segments/post_index_renderer.inc";
