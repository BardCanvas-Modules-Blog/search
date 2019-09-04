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
        
        $search  = addslashes($config->globals["search_terms"]);
        $cat_id  = addslashes($config->globals["category_id"]);
        $user    = addslashes($config->globals["user_name"]);
        $pubdate = addslashes($config->globals["pub_date"]);
        
        if( empty($search) ) throw new \Exception("Something to search must be specified.");
        
        $where = array();
        
        if( substr($search, 0, 1) == "#" )
        {
            $tag = str_replace("#", "", $search);
            $where[] = "(
                    title LIKE '%{$search}%'
                    or
                    content LIKE '%{$search}%'
                    or
                    id_post in (
                        select id_post from post_tags where tag = '$tag'
                    )
                )";
        }
        else
        {
            if( $account->level >= config::MODERATOR_USER_LEVEL )
            {
                if( strtolower(substr($search, 0, 3)) == "ip:" )
                {
                    $search = str_replace("ip:", "", $search);
                    $where[] = "creation_ip like '$search'";
                }
                elseif( strtolower(substr($search, 0, 5)) == "city:" )
                {
                    $search = str_replace("city:", "", $search);
                    $where[] = "creation_location like '$search%'";
                }
                elseif( strtolower(substr($search, 0, 8)) == "country:" )
                {
                    $search = str_replace("country:", "", $search);
                    $where[] = "creation_location like '%$search%'";
                }
                elseif( strtolower(substr($search, 0, 4)) == "isp:" )
                {
                    $search = str_replace("isp:", "", $search);
                    $where[] = "creation_location like '%$search%'";
                }
            }
            else
            {
                $where[] = "(
                    title LIKE '%{$search}%'
                    or
                    content LIKE '%{$search}%'
                    or
                    id_author in (
                        select id_account from account
                        where account.display_name like '%{$search}%'
                        or account.user_name like '%{$search}%'
                    )
                )";
            }
        }
        
        if( ! empty($cat_id) ) $where[] = "main_category = '$cat_id'";
        
        if( ! empty($user) )
            $where[] = "id_author in (
                select id_account from account
                where account.display_name like '%{$user}%'
                or account.user_name like '%{$user}%'
            )";
        
        if( ! empty($pubdate) ) $where[] = "publishing_date >= '{$pubdate} 00:00:00'";
        
        $find_params = $this->build_find_params($where);
        
        $return = $this->get_posts_data($find_params, "", "");
        
        return $return;
    }
}
