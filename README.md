# minishell

The aim of this project is to program a "simple" shell using C language.
We had to do this project in groups, so it was me & [durantecode](https://github.com/durantecode).

We had to implement 7 built-ins like in bash:

- echo (with -n option)
- cd
- pwd
- export
- unset
- env
- exit

We also had to handle redirections (<, <<, >, >>), multiple pipes (working in parallel), environment variables and some signals.

## What we learned:

It deepened our understanding of the terminal which is our working tool.
We learned a lot about file descriptors, forking processes and interprocess communication which were kinda obscur to us.
It was the first "big project" we had to do for both of us, it took us around a 2 months and a half, but in the end I think we ended up with a pretty solid shell that handles a lot of stuff, runs external programs and replicates bash in a lot of things.

If you wanna try it:

- clone this repo.
- run "make && ./minishell"
- have fun.

## Conclusion:

We had some "kill myself" moments during this project, especially with the parsing (it can be tricky), but in the end I think we really enjoyed it, and also I think it's a pretty usefull exercise as you learn a lot of things.
