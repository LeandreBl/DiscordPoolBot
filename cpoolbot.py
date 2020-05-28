import os
import subprocess
import tempfile
import re

import discord

TOKEN = os.environ["DISCORD_TOKEN"]

GUILD = os.environ["DISCORD_GUILD"]

client = discord.Client()


def load_template(template_path: str, replacements: dict):
    try:
        with open(template_path) as template_file:
            data = template_file.read()
            for key, value in replacements.items():
                data = data.replace(key, value)
            template_file.close()
            return data
    except FileNotFoundError:
        return None


def between(string: str, pattern: str):
    return f"{pattern}{string}{pattern[::-1]}"


def add_template_to_directory(
    dir_path: str, template_path: str, replacements: dict, dest_filename: str
):
    data = load_template(template_path, replacements)
    if not data:
        return False
    with open(f"{dir_path}/{dest_filename}", "w+") as file:
        file.write(data)
        file.close()
        return True
    return False


def add_template_array_to_directory(directory_path: str, array_dict: list):
    for entry in array_dict:
        if not add_template_to_directory(
            directory_path,
            entry["template_path"],
            entry["template_replacements"],
            entry["dest_filename"],
        ):
            return False
    return True


@client.event
async def on_ready():
    print(f"{client.user} has connected to Discord!")
    guild = None
    for guild in client.guilds:
        if guild.name == GUILD:
            break
    if guild:
        print(
            f"{client.user} is connected to the following guild:",
            f"{guild.name} (id: {guild.id})",
        )
    else:
        print(f"The bot is not connected to the {GUILD} server")
        exit(0)


def on_c_code(code: str):
    LANGUAGE_DIR = "c"
    FIRST_LINE = code.partition("\n")[0]
    FUNCTION_NAME = (
        FIRST_LINE.replace("//", "")
        .replace("/*", "")
        .replace("*/", "")
        .replace(" ", "")
    )
    if FUNCTION_NAME == FIRST_LINE:
        return None
    TEST_FILENAME = f"{FUNCTION_NAME}_tests.c"
    TEST_FILEPATH = f"{LANGUAGE_DIR}/{TEST_FILENAME}"
    TEMP_DIR = tempfile.TemporaryDirectory()
    EXECUTABLE_NAME = f"{TEMP_DIR.name}/{FUNCTION_NAME}_tests"
    TIMEOUT = 5.0
    templates = [
        {
            "template_path": f"{LANGUAGE_DIR}/Makefile_template",
            "template_replacements": {
                "__NAME__": EXECUTABLE_NAME,
                "__SRCS__": f"{TEMP_DIR.name}/{TEST_FILENAME}",
            },
            "dest_filename": "Makefile",
        },
        {
            "template_path": TEST_FILEPATH,
            "template_replacements": {
                "__FUNCTION_NAME__": FUNCTION_NAME,
                "__TEST_NAME__": f"{FUNCTION_NAME}_tests",
                "__ITERATIONS__": "30",
                "__CODE__": code,
            },
            "dest_filename": TEST_FILENAME,
        },
        {
            "template_path": f"{LANGUAGE_DIR}/libs/lvector/lvector.h",
            "template_replacements": {},
            "dest_filename": "lvector.h",
        },
    ]
    if not add_template_array_to_directory(TEMP_DIR.name, templates):
        return between(f"No tests available for the {FUNCTION_NAME} function", "```\n")
    process = subprocess.run(
        ["bash", "-c", f"make -C {TEMP_DIR.name}"],
        stderr=subprocess.PIPE,
        encoding="utf-8",
    )
    if process.returncode != 0:
        response = process.stderr.replace(
            f"{TEMP_DIR.name}/{TEST_FILENAME}", f"{FUNCTION_NAME}"
        )
        response = response.split("\n")
        for i in range(len(response)):
            response[i] = response[i].strip()
        error = []
        EXCEPT_KEYWORDS = ("In file included from", "make:", "from")
        for line in response:
            if line.startswith(EXCEPT_KEYWORDS):
                break
            error.append(line)
        error = "\n".join(error)
        print(process.stderr)
        response = between(f"Your function does not compile", "```\n")
        if len(error) > 0:
            response += between(f"\n{error}", "```\n")
        return response
    try:
        process = subprocess.run(
            [EXECUTABLE_NAME], stderr=subprocess.PIPE, encoding="utf-8", timeout=TIMEOUT
        )
        response = re.compile(r"(\x9B|\x1B\[)[0-?]*[ -\/]*[@-~]").sub(
            "", process.stderr
        )
        response = response.replace(
            f"{TEMP_DIR.name}/{TEST_FILENAME}", f"{FUNCTION_NAME}.c"
        )
    except subprocess.TimeoutExpired:
        response = f"Your function is taking too long (more than {TIMEOUT} seconds)"
    TEMP_DIR.cleanup()
    return between(response, "```\n")


def on_help(unused):
    return """
```
To test a function, send your sources (not the main) with this format:
(you need to provide your my_putchar function)
```
```c
```c
//my_function_name

#include <stdio.h>
#include <unistd.h>

void my_putchar(char c)
{
    write(1, &c, 1)
}

void my_function_name(const char *s)
{
    if (s != NULL)
        my_putchar(s[0]);
}
```
"""


@client.event
async def on_message(message):
    delimiters = {
        "c": {"start": "```c\n", "end": "\n```", "execute": on_c_code},
        "help": {"start": "help", "end": "", "execute": on_help},
    }
    for _, values in delimiters.items():
        if message.content.startswith(values["start"]) and message.content.endswith(
            values["end"]
        ):
            trim_start = len(values["start"])
            trim_end = -len(values["end"])
            response = values["execute"](message.content[trim_start:trim_end:])
            print(f"<-- {message.content}", f"--> {response}")
            await message.channel.send(f"{response}")


client.run(TOKEN)
