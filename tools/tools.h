/* This file is part of BabyMINDdaq software package. This software
 * package is designed for internal use for the Baby MIND detector
 * collaboration and is tailored for this use primarily.
 *
 * BabyMINDdaq is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * BabyMINDdaq is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with BabyMINDdaq.  If not, see <http://www.gnu.org/licenses/>.
 *
 */


int arg_as_int(const char *arg) {
  int my_arg;
  if (arg[0] == '0' && arg[1] == 'x')
    my_arg = strtol(arg+2, NULL, 16);
  else
    my_arg = atoi(arg);

  return my_arg;
}


int get_arg_short(const char arg_name, int argc, char **argv) {
  int i;
  for (i = 1; i < argc; i++)
    if (argv[i][0] == '-')
      if ( arg_name == argv[i][1] && argv[i][2] == '\0' && argc > i) {
        return i;
      }

  return 0;
}

int get_arg_long(const char* arg_name, int argc, char **argv) {
  int i;
  for (i = 1; i < argc; i++)
    if (argv[i][0] == '-' && argv[i][1] == '-')
      if ( strcmp(argv[i]+2, arg_name) == 0 && argc > i) {
        return i;
      }

  return 0;
}

int get_arg(const char arg_short, const char* arg_long, int argc, char **argv) {
  int x_arg = get_arg_short(arg_short, argc, argv);
  if (x_arg == 0)
    x_arg = get_arg_long(arg_long, argc, argv);

  return x_arg;
}

int get_arg_val(const char arg_short, const char* arg_long, int argc, char **argv) {
  int x_arg = get_arg(arg_short, arg_long, argc, argv);
  if (x_arg != 0 && x_arg+1 < argc)
    return x_arg + 1;

  return 0;
}
