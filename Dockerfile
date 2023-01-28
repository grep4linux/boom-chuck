# #############################################
# Description: Dockerfile to run boom-chuck in a container
# Author: Raine Curtis (curraine@amazon.com)
# Version: 1.0
# #############################################
FROM opensuse/leap AS stage1
RUN zypper up -y
RUN zypper install -y gcc make
WORKDIR /tmp
COPY boom-chuck.c .
COPY Makefile .
RUN make

FROM opensuse/leap AS stage2
WORKDIR /tmp
COPY --from=stage1 /tmp/boom-chuck /usr/local/bin/boom-chuck
CMD ["boom-chuck"]
