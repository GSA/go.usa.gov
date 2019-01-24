<?php
/**
 *  Custom response formatter for REST Server.
 *
 *   (implements abstract class RESTServerView
 *    from REST Server in Services module)
 */
/*
class RESTServerViewCSV extends RESTServerView
{
   public function render()
   {
      // check for empty result set
      if (empty($this->model))
      {
         return 'No results found.';
      }
      $csv = '';
      foreach ($this->model as $row)
      {
         // enclose rows with double quotes
         $enclosed_csv = '';
         if (is_array($row))
         {
               foreach ($row as $item)
               {
                   if(is_array($item))
                   {
                       foreach ($item as $record)
                       {
                           if (is_array($record))
                           {
                                $count = 0;
                                foreach ($record as $rec)
                                {
                                      $enclosed_csv .= '"' . str_replace('"', '""', trim($rec)) . '",';
                                      if ($rec == 'OK')
                                      {
                                         $count = 0;
                                         $enclosed_csv .= PHP_EOL . '"shortUrl","longUrl","dateCreated","URLtitle","URLdescription","URLkeywords","user_clicks",'. PHP_EOL;
                                      }
                                      $count++;
                                      if ($count == 7)
                                      {
                                         $enclosed_csv .= PHP_EOL;
                                      }
                                }
                           } else {
                                 $enclosed_csv .= '"' . str_replace('"', '""', $record) . '",';
                           }
                       }
                   } else {
                       $enclosed_csv .= '"' . str_replace('"', '""', $item) . '",';
                   }
               }
         } else {
            $enclosed_csv .= '"' . str_replace('"', '""', $row) . '",';
         }
         $csv .= substr($enclosed_csv, 0, -1);
      }
      return $csv;
   }
}

class RESTServerViewTXT extends RESTServerView
{
    public function render()
    {
        // check for empty result set
        if (empty($this->model))
        {
            return 'No results found.';
        }
        $txt = '';
        foreach ($this->model as $row)
        {
            // enclose rows with double quotes
            $simple_txt = '';
            //return print_r($row);
            if (is_array($row))
            {
                foreach ($row as $item)
                {
                    if(is_array($item))
                    {
                        foreach ($item as $record)
                        {
                            if (is_array($record))
                            {
                                $count = 0;
                                foreach ($record as $rec)
                                {
                                    if(is_array($rec))
                                    {
                                        foreach($rec as $data)
                                        {
                                            $simple_txt .= trim($data) . " ";
                                        }
                                    }
                                    $simple_txt .= trim($rec) . " ";
                                    if ($rec == 'OK')
                                    {
                                        $count = 0;
                                        $simple_txt .= PHP_EOL;
                                    }
                                    $count++;
                                    if ($count == 7)
                                    {
                                        $simple_txt .= PHP_EOL;
                                    }
                                }
                            } else {
                                $simple_txt .= trim($record). " ";
                            }
                        }
                    } else {
                        $simple_txt .= trim($item). " ";
                    }
                }
            } else {
                $simple_txt .= trim($row). " ";
            }
            $txt .= substr($simple_txt, 0, -1);
        }
        return $txt;
    }
}
*/
