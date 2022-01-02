<?php
namespace hng2_modules\search;

use hng2_repository\abstract_repository;

class search_history_repository extends abstract_repository
{
    protected $row_class                = "\\hng2_modules\\search\\search_history_record";
    protected $table_name               = "search_history";
    protected $key_column_name          = "terms";
    
    /**
     * @param search_history_record $record
     *
     * @return int
     */
    public function save($record)
    {
        global $database;
        
        $this->validate_record($record);
        
        $record->last_hit = date("Y-m-d H:i:s");
        
        $record->terms = addslashes($record->terms);
        
        return $database->exec("
            insert into search_history (
              terms, hits, last_hit
            ) values(
              '{$record->terms}', '1', '{$record->last_hit}'
            ) on duplicate key update
              hits = hits + 1
        ");
    }
    
    /**
     * @param search_history_record $record
     *
     * @throws \Exception
     */
    public function validate_record($record)
    {
        if( ! $record instanceof search_history_record )
            throw new \Exception(
                "Invalid object class! Expected: {$this->row_class}, received: " . get_class($record)
            );
    }
    
    public function get_grouped_term_counts($since = "", $min_hits = 10)
    {
        global $database;
        
        $min_hits = empty($min_hits) ? 10 : $min_hits;
        $having   = $min_hits == 1   ? "" : "having hits >= '$min_hits'";
        
        if( empty($since) )
            $query = "
                select terms, sum(hits) as hits from search_history
                group by terms
                $having
                order by terms asc
            ";
        else
            $query = "
                select terms, sum(hits) as hits from search_history
                where last_hit >= '{$since}'
                group by terms
                $having
                order by terms asc
            ";
        
        $res = $database->query($query);
        if( $database->num_rows($res) == 0 ) return array();
        
        $return = array();
        while( $row = $database->fetch_object($res) )
            $return[$row->terms] = $row->hits;
        
        return $return;
    }
}
