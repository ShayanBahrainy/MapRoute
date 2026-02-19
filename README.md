## MapRoute
MapRoute is my own implementation of traceroute, with a gui to play around with. It also looks up the ASes (Autonomous Systems) of the found IPs to "see" connections between networks. The app gives you the option of messing around yourself, or it setting a goal for you. These are in the form of "Find a path greater than X in length".

One problem however is many routes have so many ips that don't respond that the end of the route is just blank ips, up to the 30 hop limit. I've also been disappointed by how most paths are super short. So many websites are hosted on AWS, or Cloudflare that they don't return interesting routes. Even the longest paths I saw (which were universities) only passed through five or six networks at most. I really hoped to find some really long winded path, but I didn't find anything too big. It's really cool when universities, companies, or other organizations have their own networks, instead of ending on a generic CDN. For the small colleges, I'm curious who is responsible for maintaining the network, and why they think it is worth it.

The shortest path I saw was one hop IP-wise, this was while I was connected to a Cloudflare VPN so it made sense.

I do like the hacker-esque aesthetic the app ended up with. Though, that might just be because I used the default font of Raylib.

# Running
You can use the `compile_and_run.sh` script, it compiles the executable, and also gives it the necessary permissions to open raw sockets. I got tired of having to run both the commands, so I made this script.

The two executables you can build with this script are `maproute` and `main`. Maproute was how I started writing the network code for this, and it's not too interesting.

You can also build with:
```
cmake --build ./build
sudo setcap cap_net_raw+ep main
```

# Ambitions
I'd like to expand this project by adding a server to compile a list of known connections. That way, you see not only your own path, but the paths of other people. This was what I originally planned, but getting the network aspect of this working took longer than expected, so that didn't end up happening.

If I added more specific goals I think it'd be a cool daily game to see others' attempts at finding the answer, and using that as a hint to find it yourself.

A simpler feature I'd like to add is making the goal feature also consider ASes, so the goal could be find a path that makes hops through X networks. That could probably be done in less than an hour.