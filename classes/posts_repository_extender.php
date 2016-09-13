<?php
namespace hng2_modules\search;

use hng2_base\config;
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
        global $config, $account;
        
        $search = $config->globals["search_terms"];
        
        if( empty($search) ) throw new \Exception("Something to search must be specified.");
        
        $where = array();
        if( strtolower(substr($search, 0, 3)) == "ip:" && $account->level >= config::MODERATOR_USER_LEVEL )
        {
            $search = str_replace("ip:", "", $search);
            $where[] = "creation_ip like '$search'";
        }
        elseif( strtolower(substr($search, 0, 5)) == "city:" && $account->level >= config::MODERATOR_USER_LEVEL )
        {
            $search = str_replace("city:", "", $search);
            $where[] = "creation_location like '$search%'";
        }
        elseif( strtolower(substr($search, 0, 8)) == "country:" && $account->level >= config::MODERATOR_USER_LEVEL )
        {
            $search = str_replace("country:", "", $search);
            $where[] = "creation_location like '%$search%'";
        }
        elseif( strtolower(substr($search, 0, 4)) == "isp:" && $account->level >= config::MODERATOR_USER_LEVEL )
        {
            $search = str_replace("isp:", "", $search);
            $where[] = "creation_location like '%$search%'";
        }
        else
        {
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
        }
        
        $find_params = $this->build_find_params($where);
        
        return $this->get_posts_data($find_params, "", "");
    }
}
