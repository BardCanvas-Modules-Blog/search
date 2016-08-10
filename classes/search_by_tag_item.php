<?php
namespace hng2_modules\search;

use hng2_repository\abstract_record;

class search_by_tag_item extends abstract_record
{
    public $type;
    public $url;
    public $title;
    public $excerpt;
    public $thumbnail;
    
    public function set_new_id()
    {
    }
}
