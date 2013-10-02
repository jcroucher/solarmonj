#!/usr/bin/php
<?php
/**
* 
* Simple script to output solarmonj logs to www.pvoutput.org
* This script should be run as a cron job each day. It by default will upload *yesterdays* output.
*
* 02-10-13 - John Croucher <john@jcroucher.com>
* 
* No copyright. Use this how ever you want.
* 
*
*/

  $api_key = "";   // Your API key. Get this from http://pvoutput.org/account.jsp
	$system_id = ""; // This is also on the account page
	$base_path = ""; // Base path of the log files created by solarmonj. The year/month/day path will be added automatically.
	
	$yesterday = strtotime('-1 day');
	
	$day		= date("j",$yesterday);
	$month   = date("n",$yesterday);
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

			fclose($handle);
		}
	}
	
	$url = 'http://pvoutput.org/service/r2/addoutput.jsp';

	$fields = array(
							'd' => urlencode(date("Ymd",$yesterday)),
							'g' => urlencode($day_watts)
					);

					
	$post_data = "";
	
	foreach($fields as $key=>$value) 
	{ 
		$post_data .= $key.'='.$value.'&'; 
	}

	rtrim($post_data, '&');

	$ch = curl_init();

	curl_setopt($ch,CURLOPT_URL, $url);
	curl_setopt($ch,CURLOPT_POST, count($fields));
	curl_setopt($ch,CURLOPT_POSTFIELDS, $post_data);
	curl_setopt($ch, CURLOPT_HTTPHEADER, array(                                                                          
		'X-Pvoutput-Apikey: ' . $api_key,
		'X-Pvoutput-SystemId: ' . $system_id
		)
	);  

	$result = curl_exec($ch);

	curl_close($ch);