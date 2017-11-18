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
use hng2_modules\categories\categories_repository;
use hng2_modules\search\posts_repository_extender;
?>

<script type="text/javascript">
    function reset_main_search_form()
    {
        var $form = $('#main_search_form');
        
        $form.find('input[name="s"]').val('');
        $form.find('select[name="cat"] option:first').prop('selected', true);
        $form.find('input[name="user"]').val('');
        $form.find('input[name="pubdate"]').val('');
    }
</script>

<form class="search_form" name="main_search_form" id="main_search_form" method="get" action="<?= $config->full_root_path ?>/">
    <div class="form_wrapper">
        <input type="text" name="s" maxlength="100" 
               placeholder="<?= htmlspecialchars($current_module->language->index->placeholder) ?>"
               value="<?= htmlspecialchars(stripslashes($config->globals["search_terms"])) ?>">
    </div>
    <div class="additionals clearfix" style="margin-top: 10px;">
        <?
        $categories_repository = new categories_repository();
        $categories            = $categories_repository->get_as_tree_for_select();
        ?>
        <select name="cat">
            <option <? if(empty($config->globals["category_id"])) echo "selected"; ?> value="">
                &lt;<?= $current_module->language->index->all_categories ?>&gt;
            </option>
            <? foreach($categories as $id_category => $description): ?>
                <option <? if($id_category == $config->globals["category_id"]) echo "selected" ?> value="<?= $id_category ?>"><?= $description ?></option>
            <? endforeach; ?>
        </select>
        
        <input type="text" name="user" size="10" value="<?= htmlspecialchars($config->globals["user_name"]) ?>"
               placeholder="<?= htmlspecialchars($current_module->language->index->any_user) ?>">
        
        <input type="text" class="aligncenter" size="8" name="pubdate" value="<?= htmlspecialchars($config->globals["pub_date"]) ?>"
               placeholder="<?= htmlspecialchars($current_module->language->index->any_date) ?>">
        
        <span class="pull-right">
            <button type="submit">
                <?= $current_module->language->index->submit ?>
                <span class="fa fa-search"></span>
            </button>
            <button onclick="reset_main_search_form(); return false;">
                <span class="fa fa-refresh"></span>
                <?= $current_module->language->index->reset ?>
            </button>
        </span>
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
if( ! empty($config->globals["pub_date"]) )
{
    if( ! preg_match('/\d{4}-\d{2}-\d{2}/', $config->globals["pub_date"]) )
    {
        echo "
            <div class=\"framed_content state_highlight\">
                <span class=\"fa fa-info-circle\"></span>1
                {$current_module->language->messages->invalid_date}
            </div>
        ";
        
        return;
    }
    
    $parts = explode("-", $config->globals["pub_date"]);
    if( ! checkdate($parts[1], $parts[2], $parts[0]) )
    {
        echo "
            <div class=\"framed_content state_highlight\">
                <span class=\"fa fa-info-circle\"></span>2
                {$current_module->language->messages->invalid_date}
            </div>
        ";
        
        return;
    }
}

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
if($account->_is_admin && $_GET["show_query"] == "true") echo "<pre>{$repository->get_last_query()}</pre>";
$pagination_url_prefix = sprintf(
    "%s/?s=%s&cat=%s&user=%s&pubdate=%s",
    $config->full_root_url,
    urlencode($config->globals["search_terms"]),
    urlencode($config->globals["category_id"]),
    urlencode($config->globals["user_name"]),
    urlencode($config->globals["pub_date"])
    
);
$template->set("pagination_url_prefix", $pagination_url_prefix);
include "{$template->abspath}/segments/post_index_renderer.inc";
