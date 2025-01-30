# repo_check
Linux daemon to handle release changes on a repository and perform custom installation commands

## Configuration file strucure
- Default values will be used if left empty;
- All cmd `.` paths will be refered to `local_folder_path`;
- cmd with exec order < 0 will be executed BEFORE all other operations 
- refresh time may be set to values like 1s, 1m, 1h, 1d or combinations of these such as 10h30m32s
- if a command has and output, defined by the flag `has_output`, its output will be collected in the logs.
- notification_level resumes in 3 value: `m` (minimum), `i` (info) and `d` (debug), all those different levels will provide different type of insight about current operations.

### `configuration.json` structure:
```json
{
    "log_file_path" : "log\\path\\here",
    "mqtt_server_address" : "mqtt@address",
    "remote_repository" : "https://www.repo.com/myRepo",
    "local_folder_path" : "repo\\path\\here",
    "refresh_time" : "5m",
    "notification_level" : "i",
    "cmd" : [
        {
            "exec_order": 0,
            "command" : "",
            "has_output" : true
        },
        {
            "exec_order": -5,
            "command" : "rm -rf .",
            "has_output" : false
        },
        {
            "exec_order": 1,
            "command" : "echo TEST SECOND COMMAND!\n ",
            "has_output" : false
        },
        {
            "exec_order": 3,
            "command" : "echo TEST THIRD COMMAND!\n ",
            "has_output" : true
        }
    ]
}
``` 
### Default values:
- log_file_path : "" ---------> logs will be created in the current folder where repo_checker is installed, in the folder `./logs`
- mqtt_server_address : ""-----> empty mqtt means it is disabled
- remote_repository : "" ------> empty remote_repository wont make the program start 
- local_folder_path : "" -----> empty local_folder_path wont make the program start
- refresh_time : "" ---------> empty refresh_time will make the application check every 10 minutes
- notification_level : "" ----> empty value will be set to `i`, or "info" 