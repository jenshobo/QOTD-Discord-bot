# QOTD Discord bot

Bot made for use in the [Peer2Peer](https://www.sv-peer2peer.nl/) discord. This is made with the mindset of a Linux server, where time based tasks can be created using [systemd](https://github.com/systemd/systemd) where both apps can be setup to be run as services, manager_app can be setup to run on startup while update_app can be setup to run periodically using timers. ***I take it Windows also has something like this but I don't use Windows ~Jens***.

Both apps are to be run under one Discord bot. These programs are as follows:

### manager_app

This app is to be run at all times to allow management to add and remove the questions in the queue. This app should not be avaibable to other users in the Discord server.

### update_app

This app is to be run ones at the point in time a question is to be posted, after this, this app will automatically close again after posting, please note that this program will also close if failed. Preferably the program should be run ones a day, however more is possible by altering the setup. However do keep in mind Discord will only allow the app to start up to 1000 times every 24 hours.

## privilages

To run on a physical server and on the Discord server it is importent to grant the bot specific privilages. On the physical server where the apps will run it is important the programs are allowed to read and write to the file ```QOTD_queue.json``` which is to be placed in the same folder as the apps. To allow the bot to run in Discord it should be granted the following privilages buring the bot's creation in the [Discord developer environment](https://discord.com/developers/applications) OAuth2 URL generator, this URL can be pasted into a browser to invite the bot into the server:

| Catagory | permission |
| - | - |
| scopes | message.read |
| scopes | bot |
| text permissions | Send Messages |

Then for it to run properly in Discord it should have ```View Channel``` and ```Send Messages``` access to two channels, this being the private channel where questions are managed with the commission whom is responsible for this and the QOTD channel where it'll send all it's items from the queue. Make sure to add the latter channel's ID to the ```QOTD_queue.json```. If you use an official ```.deb``` install file for this project, this value will be overwritten by the install process.

In order for both apps to be able to see the bot, the bot's token should be placed in the ```QOTD_queue.json``` file as well. Just like before, during an official install the value will be overwritten by the install process.

If you plan on not compiling yourself and instead just use the official ```.deb``` file, setting these values is not needed as you'll be asked about them during the install process.

## commands

The manager app knows a number of commands in order to interface the queue with the people responible for managing it. These commands are as follows:

| Command | Description |
| - | - |
| ```/add``` | Add a question to the back of the queue. |
| ```/addprio```| Add a question to the back of the priority queue. |
| ```/remove``` | Remove a question BY INDEX. |
| ```/list``` | List the current queue with index. |
| ```/help``` | A short explaination about the functionality of the bot. |

As this bot is made for [Peer2Peer](https://www.sv-peer2peer.nl/) some text in the bot might reflect this.

## packages

In order to succesfully compile these apps the following programs are needed to be installed and/or downloaded:

| App | Where |
| - | - |
| [D++ Discord](https://dpp.dev/) | Global install |
| [nlohmann json.hpp](https://github.com/nlohmann/json/blob/develop/single_include/nlohmann/json.hpp) | place file in ```lib``` folder |

## install guide

To completely set up the program and bot the progress boils down to two steps: setting up the Discord bot and compiling the code.

### setup Discord bot

We assume you at least have a basic understanding of mananing a Discord server.

To start you need to setup the Discord bot that'll post the QOTD. To do this you'll need to go the the [Discord developer environment](https://discord.com/developers/applications). Log in using your Discord account, then you should be able to create a new application.

Ones created you should be able to enter it. In the tab ```bot``` you should be able to reset your token, save this token as you'll need it during the install progress of the program later.

You then need to invite the bot over to your Discord, to do this you need to go to the tab ```OAuth2```. In here you need to use the ```OAuth2 URL Generator``` to create a URL, please make sure to check ```message.read``` and ```bot```, ```bot``` should open another list of checkmarks below, in here, check ```Send Messages```. Then at the bottom there should be a generated URL, copy and paste this into a new browser tab to invite the bot over to your server.

In Discord you need to give the bot permission to view the channel and send messages in two channels, one where you will manage the bot behind the scenes and one where the bot will post the QOTD for every other user to see. The bot does not need permission to use any other channel (as a matter of fact I recommend not allowing the bot in any other channels).

Lastly the bot will need the channel ID of the channel you want it to send the QOTD to. You can get this by right clicking the channel and press ```copy channel ID```. Save this ID for later as you need to during the install progress for the program.

### compile guide

To compile the program you'll need to all listed packages downloaded and/or installed. Furthermore please create a folder called build in the working directory. Then execute the following commands to compile the two programs, these will be saved to the build folder.

```sh
g++ -std=c++17 -Ihdr/queue_update -Ilib -Ihdr -o build/update_app src/queue_update/main.cpp src/json.cpp -ldpp
g++ -std=c++17 -Ihdr/queue_manager -Ilib -Ihdr -o build/manager_app src/queue_manager/main.cpp src/json.cpp -ldpp
```

Next you'll need to create a json file in which the queue will be stored among a few other things, this file looks like follows. You'll need to manually add the discord bot token you've generated earier. Offset is just a number used to index all the items in the queue, if you have never done a QOTD in Discord before, leave this as 1. qoft-channel-id is the id of the channel the bot will post all the questions.

```json
{
    "default": "",
    "discord-bot-token": "",
    "offset": 1,
    "qotd-channel-id": 0,
    "qotd-alert-channel-id": 0,
    "prioqueue": [],
    "queue": []
}
```

This file needs to be saved in the same folder as the two programs, as both programs interact with it. The file should be named ```QOTD_queue.json``` Avoid sharing the contents of this file as the discord-bot-token is considered sensitive information.

Next simply run the manager_app program indefinitively and run the update_app ones a day to see the program in action.

### install program (precompiled)

THIS PROCESS HAS BEEN TEMPORARILY REMOVED, if you want to use .deb, please check back at a later date.

To install the program you can either use a webbrowser to download and install the required version (lastest is recommended) by going to the [GitHub release page](https://github.com/jenshobo/QOTD-Discord-bot/releases). However it is usually easier to just used the terminal. To download the package, you can use the following command:

```sh
wget https://github.com/jenshobo/QOTD-Discord-bot/releases/download/v1.1/QOTD-Discord-bot-1.1.deb
```

Regardless of download choice, to install the package it is recommened to use a terminal. Ones in the download directory, please use the following command to start the install progress:

```sh
dpkg -i QOTD-Discord-bot-1.1.deb
```

The install progress will ask about the bot token which you generated earlier. the start index, each question is indexed, if your channel hasn't had any QOTD's yet, just put in ```1```. And the channel ID you also fetched earlier.

After this the bot should be up and running and you can execute the commands in the allowed channels on Discord.

To finish you can remove the ```.deb``` file as it is no longer needed. You can do this by executing the following command:

```sh
rm QOTD-Discord-bot-1.1.deb
```
