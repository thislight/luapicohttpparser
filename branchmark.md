# Branchmark Results

## Record 0

- On AMD Ryzen 4600U, 12GiB memory
- Generating data takes around 8 minutes
- Eat memory up to 7.5GiB
- lphr.c is faster than lphr with ~20%
- The way to processing strings in branchmark for lphr.c takes more memory (~60%)
- lphr uses strbuffer:concat()


Raw output:
````
generating data for 10000000 request(s)...
used memeory (Kbytes):	4974295.7890625
---- lphr.parse_request ----
lphr.parse_request used 181 sec, speed 55248.61878453 per second.
---- lphr.c.parse_request ----
lphr.c.parse_request used 146 sec, speed 68493.150684932 request(s) per second.
````

## Record 1
- On AMD Ryzen 4600U, 12GiB memory
- Generating data takes around 8 minutes
- Eat memory up to 7.6GiB
- lphr.c is faster than lphr with ~10%
- lphr uses strbuffer:qconcat()
- the gc configuration changed during setting up dataset

Raw output:
````
generating data for 10000000 request(s)...
used memeory (Kbytes):	3346995.3154297
---- lphr.parse_request ----
lphr.parse_request used 172 sec, speed 58139.534883721 per second.
---- lphr.c.parse_request ----
lphr.c.parse_request used 159 sec, speed 62893.081761006 request(s) per second.
````

