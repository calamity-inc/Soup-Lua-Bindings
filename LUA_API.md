# Soup Lua Bindings

Although the Lua bindings themselves are compatible with vanilla Lua, some of the code samples provided here require [Pluto](https://plutolang.github.io/docs/Introduction/).

## Data

### *string* soup.json.encode(*any* data, *bool* pretty = false)

```Lua
local data = {
    {
        key = "Soup",
        value = true
    }
}
print(soup.json.encode(data))
```

### *any* soup.json.decode(*string* json)

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

## I/O

### *userdata* soup.FileReader(*string* path)

### *userdata* soup.StringReader(*string* data)

### *userdata* soup.ZipReader(*userdata* seekable_reader)

Note that the ZipReader takes a pointer to the reader instance, so a ZipReader instance must not reach scopes that the reader instance can't reach.

```Lua
local fr = soup.FileReader("test.zip")
local zr = soup.ZipReader(fr)
for _, f in zr:getFileList() do
    print(f.name .. ": " .. zr:getFileContents(f))
end
```

## Math

### *userdata* soup.Matrix()

Returns a soup::Matrix instance which provides `setPosRotXYZ` method and multiplication operator taking soup::Vector3.

<h3>
    <i>userdata</i> soup.Vector3()<br>
    <i>userdata</i> soup.Vector3(<i>number</i> x, <i>number</i> y, <i>number</i> z)
</h3>

Returns a soup::Vector3 instance.

## Util

### *string* soup.string.fromFile(*string* path)

### *int* soup.version_compare(*string* a, *string* b)
