<?php
/**
 * Search facility
 *
 * @package    BardCanvas
 * @subpackage search
 * @author     Alejandro Caballero - lava.caballero@gmail.com
 * 
 * @var config   $config
 * @var template $template
 */

use hng2_base\config;
use hng2_base\template;
use hng2_modules\search\search_history_record;
use hng2_modules\search\search_history_repository;

include "../config.php";
include "../includes/bootstrap.inc";

$config->globals["search_terms"] = str_replace("\"", "", trim(stripslashes($_REQUEST["s"])));
$config->globals["category_id"]  = trim(stripslashes($_REQUEST["cat"]));
$config->globals["user_name"]    = trim(stripslashes($_REQUEST["user"]));
$config->globals["pub_date"]     = trim(stripslashes($_REQUEST["pubdate"]));

if( ! empty($config->globals["search_terms"]) )
{
    $history_repository = new search_history_repository();
    
    $terms      = strtolower($config->globals["search_terms"]);
    $hash       = md5($terms);
    $cookie_key = "{$config->website_key}_s_{$hash}";
    if( empty($_COOKIE[$cookie_key]) )
    {
        if( $account->level < config::MODERATOR_USER_LEVEL )
            $history_repository->save(new search_history_record(array(
                "terms" => $terms
            )));
        
        setcookie($cookie_key, $hash, strtotime("now + 30 minutes"), "/", $config->cookies_domain);
    }
}

$template->set("page_tag", "search_results");
$template->page_contents_include = "index.inc";
$template->set_page_title(replace_escaped_vars(
    $current_module->language->index->title, '{$terms}', htmlspecialchars($config->globals["search_terms"])
));
include "{$template->abspath}/main.php";
