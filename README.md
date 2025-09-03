# QOTD Discord bot

Bot made for use in the [Peer2Peer](https://www.sv-peer2peer.nl/) discord. These bots are made with the mindset of a Linux server, where time based tasks can be created using [systemd](https://github.com/systemd/systemd) where both can be setup to be run as services, manager_app can be setup to run on startup while update_app can be setup to run periodically using timers. ***I take it Windows also has something like this but I don't use Windows ~Jens***.

Both programs are to be run under one bot. The bot consists of these two programs:

### manager_app

This app is to be run at all times to allow management to add and remove the questions in the queue. This app should not be avaibable to other users.

### update_app

This app is to be run ones at the point in time a question is to be posted, after this, this app will automatically close again, preferably the program should be run ones a day, however more is possible by altering the setup. However do keep in mind Discord will only allow the app to start up to 1000 times every 24 hours.

## packages

In order to succesfully compile these apps the following programs are needed to be installed and/or downloaded:

| App | Where |
| - | - |
| [D++ Discord](https://dpp.dev/) | Global install |
| [nlohmann json.hpp](https://github.com/nlohmann/json/blob/develop/single_include/nlohmann/json.hpp) | file in ```lib``` folder |


## privilages

To run on a physical server and on the Discord server it is importent to grant the bot specific privilages. On the physical server where the apps will run it is important the programs are allowed to read and write to the file ```QOTD_queue.json``` which is to be placed in the same folder as the apps. To allow the bot to run in Discord it should be granted the following privilages buring the bot's creation in the [Discord developer environment](https://discord.com/developers/applications) OAuth2 URL generator, this URL can be pasted into a browser to invite the bot into the server:

| Catagory | permission |
| - | - |
| scopes | message.read |
| scopes | bot |
| text permissions | Send Messages |

Then for it to run properly in Discord it should have ```View Channel``` and ```Send Messages``` access to two channels, this being the private channel where questions are managed with the commission whom is responsible for this and the QOTD channel where it'll send all it's items from the queue. Make sure to add the latter channel's ID to the ```credentials.h``` file.

In order for both apps to be able to see the bot, the bot's token should be placed in the ```credentials.h``` file as well.

For savety sake is this file added to the .gitignore, the file without credentials looks like this, it should be placed in the hdr folder:

```C++
#ifndef CREDENTIALS_H
#define CREDENTIALS_H

#include <dpp/dpp.h>

const std::string DISCORD_BOT_TOKEN = ""; // Replace with bot token
const dpp::snowflake DISCORD_CHANNEL_ID = 0; // Replace with QOTD channel ID

const std::string QUEUE_FILE_NAME = "QOTD_queue.json";

#endif
```

## install guide

***If you ask nice enough I'll consider creating an install file***
