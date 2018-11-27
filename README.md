ESP32 GitHub OTA Template
=========================

[![Build Status](https://api.travis-ci.org/pathob/esp32-github-ota.svg?branch=master)](https://travis-ci.org/pathob/esp32-github-ota)

This is a template and demo for how you can use GitHub together with Travis CI
to run automated updates over the air on your ESP32 devices.
For open source projects, everything is completely free of charge.

This approach currently assumes that you have one separate repository for every type of project.
All you basically need is tagging of your commits (`git tag <TAG>`) and the GitHub repo slug.
The repo slug is composed of the user or organisation name and your the repository name, e.g. `name/repo`.

First, create a new project from scratch or by using this template.

```bash
export GITHUB_REPO_NAME="new_esp32_project"
git clone --recursive https://github.com/pathob/esp32-github-ota.git ${GITHUB_REPO_NAME}
cd ${GITHUB_REPO_NAME}
```

The `--recursive` option ensures that also the submodules are checked out.
These submodules include a 'connectivity' component that implements the logic for the OTA updates.

You can also use an existing project and add the capability for OTA updates from GitHub.
Simply add the 'connectivity' component by executing the following command from the project root.

```bash
git submodule add https://github.com/pathob/esp32-connectivity.git components/connectivity
```

Also make sure to copy the `.travis.yml` file from the following repository to your project root:
https://github.com/pathob/esp32-github-ota

You now have a very simple project with (theoretical) OTA update capabilities.
Now, you will need the following:


GitHub Project Repository
-------------------------

Create a new GitHub project repository if needed: https://github.com/new
Then go back to your terminal and set your new origin remote.

```bash
export GITHUB_USER=octocat
export GITHUB_REPO_SLUG="${GITHUB_USER}/${GITHUB_REPO_NAME}"
export GITHUB_REPO_URL="https://github.com/${GITHUB_REPO_SLUG}.git"
git remote set-url origin ${GITHUB_REPO_URL}
```


GitHub Token
------------

Go to https://github.com/settings/tokens/new to generate a new personal access token.
Only check the `public_repo` scope and save the token with an appropriate description.
If you want to reuse that token, use a generic description and remember to store that token for later use.
If you only want to use that token for the new repository, you should give it a repository related description
and you can only keep it in your clipboard.

Then execute the folling command:

```bash
export GITHUB_TOKEN=...
```


Travis Setup
------------

Most of the following steps could also be done using the Travis web interface,
but we will do everything using the command line interface.

Install the Travis CLI using these installation instructions:
https://github.com/travis-ci/travis.rb#installation

Then switch back to your console again.

You will not need a Travis account, instead Travis uses your GitHub account.
If you never logged in to Travis execute the following command:

```bash
travis login --org --github-token ${GITHUB_TOKEN}
```

If the login was successful, you will see a message like "Successfully logged in as ...".
Then, execute the following command to enable Travis builds for your GitHub repository.
In this step you will have to verify the auto-detected repo slug by typing 'yes'.

```bash
travis enable
```

Now, you need to store the (encrypted) GitHub token in your Travis configuration.
Overwrite that deployment API key in `.travis.yml` by executing the following command:

```bash
travis encrypt ${GITHUB_TOKEN} --add deploy.api_key
```

That's it from the configuration part.
Now you need to flash an application with some OTA meta data to an ESP32 of your choice.


Git Push Tag
------------

Commit all the neccessary project files. These include:

```bash
components             # your custom components
main                   # your main application code
Makefile               # or CMakeLists.txt
sdkconfig.defaults     # defaults for SDK configuration
```

Add all your wanted files and commit them:

```bash
git add ...
git commit -m "Setup travis"
```

Then create your first tag:

```bash
export GIT_TAG=0.0.1
git tag ${GIT_TAG}
git push origin ${GIT_TAG}
```

Travis should now start the build and deploy the resulting binaries to the repositories release section.


Flashing OTA Application
------------------------

In order to be able to download the wanted binaries, the flashed application needs to know your repo slug and the tag.
For this, you need to export the following two environment variables (the repo slug variable may already be defined):

```bash
export GITHUB_REPO_SLUG=0.0.0
export GITHUB_TAG=0.0.0
```

It is important to use the tag 0.0.0, if you use the same tag as you pushed, you will not see any update.

Then configure Wi-Fi for station mode in the menuconfig tool.

```bash
make clean
make defconfig
make menuconfig
```

Go to 'Connectivity config' -> 'Wi-Fi config' -> 'Station mode config' and fill out SSID and password.

Also make sure your code looks similar to the following:

```c
void app_main()
{
    // ...

    WIFI_init(WIFI_MODE_STA , NULL);
    OTA_init();

    // ...
}
```

Last but not least execute the following command:

```bash
make flash monitor
```

If you've followed all the steps carefully, you should now see the app updating to the newer release.
