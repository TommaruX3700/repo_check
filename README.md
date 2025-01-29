# repo_check
Linux daemon to handle release changes on a repository and perform custom installation commands

## Configuration file strucure
- Default values will be used if left empty;
- All cmd `.` paths will be refered to `local_folder_path`;
- cmd with exec order < 0 will be executed BEFORE all other operations 
- refresh time may be set to values like 1s, 1m, 1h, 1d, 1m or combinations of these such as 10h30m32s

### `configuration.json` structure:
```json
{
    "log_file_path" : "log\\path\\here",
    "mqtt_server_address" : "mqtt@address",
    "remote_repository" : "https://www.repo.com/myRepo",
    "local_folder_path" : "repo\\path\\here",
    "refresh_time" : "5m",
    "cmd" : [
        {
            "exec_order": 0,
            "command" : ""
        },
        {
            "exec_order": -5,
            "command" : "rm -rf ."
        },
        {
            "exec_order": 1,
            "command" : "echo TEST SECOND COMMAND!\n "
        },
        {
            "exec_order": 3,
            "command" : "echo TEST THIRD COMMAND!\n "
        }
    ]
}
``` 
### Default values:
- log_file_path : "" ---------> logs will be created in the current folder where repo_checker is installed, in the folder `./logs`
- mqtt_server_address : ""-----> empty mqtt means it is disabled
- remote_repository : "" ------> empty remote_repository wont make the program start 
- local_folder_path : "" -----> empty local_folder_path wont make the program start
- refresh_time : "" ---------> emptu refresh_time will make the application check every 10 minutes