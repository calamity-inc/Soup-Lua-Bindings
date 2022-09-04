# Soup Lua Bindings

Although the Lua bindings themselves are compatible with vanilla Lua, some of the code samples provided here require [Pluto](https://plutolang.github.io/docs/Introduction/).

## *userdata* soup.netIntel.getAsByIp(*int|string* ip)

```Lua
if (as := soup.netIntel.getAsByIp("1.1.1.1")):isValid() then
    print(as.number .. ", " .. as.handle .. ", " .. as.name .. ", " .. tostring(as:isHosting()))
end
```

## *userdata* soup.netIntel.getLocationByIp(*int|string* ip)

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
