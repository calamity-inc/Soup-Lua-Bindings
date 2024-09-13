# Soup Lua Bindings - Deprecated Functions

This is a continuation of [LUA_API.md](LUA_API.md) with functions that still exist but are planned to be removed.

## Audio

### *userdata* soup.audDevice.getDefault()

audDevice instances have `getName` and `open` methods.

Calling `:open` returns an audPlayback instance. audPlayback instances have an `isPlaying` method.

### *userdata* soup.audMixer()

audMixer instances have a `stop_playback_when_done` field and `setOutput` and `playSound` methods.

### *userdata* soup.audWav(*userdata* reader)

Note that the audWav takes a pointer to the reader instance, so a audWav instance must not reach scopes that the reader instance can't reach.

audWav instances have a read-only `channels` field.

**Example: Playing a WAV file**

```Lua
local fr = soup.FileReader([[path_to_wav_file]])
local wav = soup.audWav(fr)
local dev = soup.audDevice.getDefault()
print("Playing on " .. dev:getName())
local pb = dev:open(wav.channels)
local mix = soup.audMixer()
mix.stop_playback_when_done = true
mix:setOutput(pb)
mix:playSound(wav)
while pb:isPlaying() do sleep(10) end
```

## Net

### *userdata* soup.netIntel.getAsByIp(*int|string|userdata* ipAddr)

```Lua
if (as := soup.netIntel.getAsByIp("1.1.1.1")):isValid() then
    print(as.number .. ", " .. as.handle .. ", " .. as.name .. ", " .. tostring(as:isHosting()))
end
```

### *userdata* soup.netIntel.getLocationByIp(*int|string|userdata* ipAddr)

```Lua
if (loc := soup.netIntel.getLocationByIp("1.1.1.1")):isValid() then
    print(loc.city .. ", " .. loc.state .. ", " .. loc.country_code)
end
```

### *string|nil* soup.getCountryName(*string* country_code, *string* language_code = "EN")

```Lua
print(soup.getCountryName("US", "EN"))
print(soup.getCountryName("US", "DE"))
print(soup.getCountryName("US", "FR"))
print(soup.getCountryName("US", "ZH-CN"))
```

### *userdata* soup.IpAddr(*int|string|userdata* ipAddr)

```Lua
local ip = soup.IpAddr("1.1.1.1")
print(tostring(ip) .. ", " .. ip:getReverseDns())
```
