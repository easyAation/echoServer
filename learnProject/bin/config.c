#include <stdio.h>
#include "config.h"
void init_conf(struct echo_conf_t* conf)
{
	FILE* fp = NULL;
	if((fp = fopen(CONFIG_FILE_NAME , "r")) == NULL)
	{
		perror("fail to open conf file. now used default config.");
		default_conf(conf);
		return ;
	}
    
	char line[128] , parse_name[128] , parse_value[128];
	while(fgets(line , 128 , fp) != NULL)
	{
		sscanf(line, "%s %s", parse_name , parse_value);
		if(strcmp(parse_name , "PORT") == 0)
			conf->port = atoi(parse_value);
		else if(strcmp(parse_name , "THREAD_NUM") == 0)
			conf->thread_num = atoi(parse_value);
		else if(strcmp(parse_name , "LISTEN_NUM") == 0)
			conf->listen_num  = atoi(parse_value);
		else if(strcmp(parse_name , "EVENT_NUM") == 0)
			conf->event_list_num  = atoi(parse_value);
		else if(strcmp(parse_name , "HEART_TIME") == 0) {			
			conf->max_interval_time.tv_sec = atoi(parse_value);
			conf->max_interval_time.tv_nsec = 0;
		}
	}
	fclose(fp);
	return ;
}

void default_conf(struct echo_conf_t* conf)
{
	conf->port = default_port;
	conf->thread_num = default_thread_num;
	conf->listen_num = default_listen_num;
	conf->event_list_num = default_event_list_num;
	conf->max_interval_time.tv_sec = default_heart_time;
	conf->max_interval_time.tv_nsec = 0;
	return ;
}
 	
