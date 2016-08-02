<?php
namespace hng2_modules\search;

use hng2_base\repository\abstract_record;

class search_history_record extends abstract_record
{
    public $terms;
    public $hits;
    public $last_hit;
    
    public function set_new_id()
    {
        throw new \Exception("Method not needed");
    }
}
