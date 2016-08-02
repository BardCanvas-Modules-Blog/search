<?php
/**
 * Search results
 *
 * @package    BardCanvas
 * @subpackage search
 * @author     Alejandro Caballero - lava.caballero@gmail.com
 *             
 * @var config $config
 */

use hng2_base\config;
?>

<h1 class="clearfix for_addition">
    <?= replace_escaped_vars( $current_module->language->index->title, '{$terms}', htmlspecialchars($config->globals["search_terms"]) ) ?>
</h1>
