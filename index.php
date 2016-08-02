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

$config->globals["search_terms"] = trim(stripslashes($_REQUEST["s"]));

$history_repository = new search_history_repository();

$hash       = md5($config->globals["search_terms"]);
$cookie_key = "{$config->website_key}_s_{$hash}";
if( empty($_COOKIE[$cookie_key]) )
{
    $history_repository->save(new search_history_record(array(
        "terms" => $_REQUEST["s"]
    )));
    
    setcookie($cookie_key, $hash, 0, "/", $config->cookies_domain);
}

$template->set("page_tag", "search_results");
$template->page_contents_include = "index.inc";
$template->set_page_title(replace_escaped_vars(
    $current_module->language->index->title, '{$terms}', htmlspecialchars($config->globals["search_terms"])
));
include "{$template->abspath}/main.php";
