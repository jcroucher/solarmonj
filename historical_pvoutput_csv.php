<?php
/**
* 
* Script to run through any old logs and export to a format to be pasted into www.pvoutput.org. I created this file because the API is rate limited to 60 add requests per hour.
*
* Usage: 
* php historical_pvoutput_csv.php > source_file.csv
*
* You can then copy and paste the CSV data into the pvoutput csv data import http://pvoutput.org/load.jsp
* Note that there is a limit to how many can be uploaded at a time.
*
* 02-10-13 - John Croucher <john@jcroucher.com>
* 
* No copyright. Use this how ever you want.
* 
*
*/

  $base_path = ""; // Base path of the log files created by solarmonj. The year/month/day path will be added automatically.
  $number_of_days = 400; // How many days in the past to export
	
	for($i=1;$i<$number_of_days;$i++)
	{
		$yesterday = strtotime('-' . $i . ' day');
		
		$day		= date("j",$yesterday);
		$month 	= date("n",$yesterday);
		$year 	= date("Y",$yesterday);

		$path 	 = "$base_path/$year/$month/$day.csv";
		
		$day_watts = 0;
		
		if( file_exists($path) )
		{
			if (($handle = fopen($path, "r")) !== FALSE) 
			{
				
				while (($cdata = fgetcsv($handle, 1000, ",")) !== FALSE) 
				{
					$c_hour = date("H",$cdata[0]);
					
					$data_points[$c_hour][] = array("temperature" 		=> $cdata[1],
																"energyCurrent" 	=> $cdata[2],
																"day_watts" 		=> $cdata[3],
																"lifetime_watts" 	=> $cdata[4],
																"voltageDc" 		=> $cdata[5],
																"voltageAc" 		=> $cdata[6],
																"frequency" 		=> $cdata[7],
																"current" 			=> $cdata[8]
																);
					
					if( strpos($cdata[3],"e") === false && $cdata[3] > 0 )
					{
						$day_watts		= intval($cdata[3] * 1000);
					}
				}

				echo "\n" . date("d/m/y",$yesterday) . "," . $day_watts;
				fclose($handle);
			}
		}
	}