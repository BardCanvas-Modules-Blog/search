<?php
namespace hng2_modules\search;

use hng2_modules\posts\posts_repository;

class posts_repository_extender extends posts_repository
{
    /**
     * @return \hng2_modules\posts\posts_data
     * 
     * @throws \Exception
     */
    public function search()
    {
        global $config;
        
        $search = $config->globals["search_terms"];
        
        if( empty($search) ) throw new \Exception("Something to search must be specified.");
        
        $where        = array();
        $search_type  = stristr($search, '"') !== false ? "phrase" : "anything";
        $search       = str_replace('"', "", $search);
        $search_array = explode(" ", $search);
        $columns      = array("title", "content");
        
        if( $search_type == "phrase" )
        {
            $terms = array("
                id_author in (
                    select id_account from account
                    where account.display_name like '%{$search}%'
                    or account.user_name like '%{$search}%'
                )
            ");
            foreach($columns as $col) $terms[] = "($col LIKE '%{$search}%')";
            $where[] = "(" . implode("\nOR ", $terms) . ")\n";
        }
        else
        {
            $terms = array("
                id_author in (
                    select id_account from account
                    where account.display_name like '%{$search}%'
                    or account.user_name like '%{$search}%'
                )
            ");
            foreach($columns as $col) $terms[] = "($col LIKE '%" . implode("%' AND $col LIKE '%", $search_array)."%')";
            $where[] = "(" . implode("\nOR ", $terms) . ")\n";
        }
        
        $find_params = $this->build_find_params($where);
        
        return $this->get_posts_data($find_params, "", "");
    }
}
