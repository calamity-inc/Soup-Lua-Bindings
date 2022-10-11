# Soup Lua Bindings

Although the Lua bindings themselves are compatible with vanilla Lua, some of the code samples provided here require [Pluto](https://plutolang.github.io/docs/Introduction/).

## *string* soup.json.encode(*any* data, *bool* pretty = false)

```Lua
local data = {
    {
        key = "Soup",
        value = true
    }
}
print(soup.json.encode(data))
```

## *any* soup.json.decode(*string* json)

## *userdata* soup.netIntel.getAsByIp(*int|string|userdata* ipAddr)

```Lua
if (as := soup.netIntel.getAsByIp("1.1.1.1")):isValid() then
    print(as.number .. ", " .. as.handle .. ", " .. as.name .. ", " .. tostring(as:isHosting()))
end
```

## *userdata* soup.netIntel.getLocationByIp(*int|string|userdata* ipAddr)

```Lua
if (loc := soup.netIntel.getLocationByIp("1.1.1.1")):isValid() then
    print(loc.city .. ", " .. loc.state .. ", " .. loc.country_code)
end
```

## *string|nil* soup.getCountryName(*string* country_code, *string* language_code = "EN")

```Lua
print(soup.getCountryName("US", "EN"))
print(soup.getCountryName("US", "DE"))
print(soup.getCountryName("US", "FR"))
print(soup.getCountryName("US", "ZH-CN"))
```

## *userdata* soup.IpAddr(*int|string|userdata* ipAddr)

```Lua
local ip = soup.IpAddr("1.1.1.1")
print(tostring(ip) .. ", " .. ip:getReverseDns())
```

## *userdata* soup.Matrix()

Returns a soup::Matrix instance which provides `setPosRotXYZ` method and multiplication operator taking soup::Vector3.

<h2>
    <i>userdata</i> soup.Vector3()<br>
    <i>userdata</i> soup.Vector3(<i>number</i> x, <i>number</i> y, <i>number</i> z)
</h2>

Returns a soup::Vector3 instance.
