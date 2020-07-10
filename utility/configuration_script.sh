# CHECK FIRST IF THE GROUP EXISTS
# IF NOT CREATE THE GROUP
useradd --system --no-log-init --user-group --shell /sbin/nologin maxrf
usermod -L maxrf

# ADD THE CURRENT USER TO THE MAXRF GROUP
usermod --append --groups maxrf $USER

# CHECK IF THE /usr/local/etc/maxrf directory exists

# ADD THE DIRECTORIES AND THE CONFIGURATION FILES
mkdir --parents /usr/local/etc/maxrf/motors.d
touch /usr/local/etc/maxrf/maxrf.conf


chown --recursive maxrf:maxrf ./maxrf
chmod --recursive u=rwx,g=rwx,o=rx ./maxrf
