<?php
   
   function _load() { 
   
        $file = end(glob("c:/program files/core temp/*.csv"));
        # change to your core temp's directory
        
        return file_get_contents($file);
   }
   
   function _parse($index) {
   
        $lines = explode("\r\n", _load());
        $line = $lines[count($lines) - 2];
        $values = explode(",", $line);
        
        return $values[$index];
   }

   function temp()      { return _parse(1); }   
   function low_temp()  { return _parse(4); }
   function high_temp() { return _parse(5); }
   function core_load() { return _parse(6); }   
   function speed()     { return _parse(7); }   

   function processor() {
        
        preg_match("/Processor:,(.*)/", _load(), $return);
        
        return end($return);
   }
   
   main();