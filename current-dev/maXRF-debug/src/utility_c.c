/*
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#include "include/utility_c.h"

bool check_CAEN_USB_driver()
{
  bool result = false;
  // Assuming kernel modules are found in /proc/modules
  FILE* modules = fopen("/proc/modules", "r");
  if (modules != NULL)
  {
    size_t len = 0;
    char* line = NULL;
    char* CAEN_module = "CAENUSBdrvB";

    ssize_t read;
    while ((read = getline(&line, &len, modules)) != -1)
    {
      if (len < (size_t)(read + 1))
      {
        len = (size_t)(read + 1);
      }

      if (strstr(line, CAEN_module) != NULL)
      {
        //TODO inform it's been found
        result = true;
        break;
      }
    }
    free(line);
  }
  fclose(modules);
  return result;
}

bool check_serial_port_access(const char* port)
{
  bool result = false;

  // Assuming a Linux-arch where ports are files in /dev
  char* base_dir = "/dev/";
  DIR* dir = opendir(base_dir);
  struct dirent*  entry = NULL;

  // Search the directory to see if the port is open
  while ((entry = readdir(dir)) != NULL)
  {
    // If the port is found check access rights
    if (strcmp(port, entry->d_name) == 0)
    {
      char* full_file_name;
      size_t alloc_size = strlen(entry->d_name) + strlen(base_dir) + 1;
      if ( (full_file_name = (char*)calloc(1, alloc_size)) == NULL )
      {
        // TODO Inform of error
        return result;
      }
      strcat(full_file_name, base_dir);
      strcat(full_file_name, entry->d_name);

      // We use the POSIX library to get info about the file
      struct stat info;
      stat(full_file_name, &info);
      free(full_file_name);

      // If group owning the file has no read-write rights,
      // prompt the user to change them.
      if ( (info.st_mode & (S_IRGRP | S_IWGRP))  == 0)
      {
        // TODO Prompt change
        return result;
      }

      // Verify the user is in the group owning the file
      // If not prompt root access to add the user to the group

      // Get owning group info via POSIX libraries
      struct group* owning_grp = getgrgid(info.st_gid);
      // Get user info
      uid_t user_id = getuid();
      struct passwd* user_info = getpwuid(user_id);

      int ngroups_max = (int)sysconf(_SC_NGROUPS_MAX);
      gid_t* groups = (gid_t*)calloc((size_t)ngroups_max, sizeof (gid_t));
      if (groups == NULL) {
        // TODO inform of error
        return result;
      }

      if (getgrouplist(user_info->pw_name, user_info->pw_gid, groups, &ngroups_max) == -1)
      {
        fprintf(stderr, "getgrouplist() returned -1; ngroups = %d\n",
                ngroups_max);
        return result;
      }

      int ind = 0;
      while (ind < ngroups_max)
      {
        if (owning_grp->gr_gid == groups[ind])
        {
          result = true;
          break;
        }
        ++ind;
      }
      free(groups);
      if (ind == ngroups_max)
      {
        // Prompt the user to add
      }
    }
  }
  closedir(dir);
  return result;
}

