char *get_distro_id() {
  FILE *fp = fopen("/etc/os-release", "r");
  if (!fp) {
    return NULL;
  }

  char line[256];
  char *start, *end;

  while (fgets(line, sizeof(line), fp)) {
    if (strstr(line, "ID=")) {
      start = strchr(line, '=');
      if (start) {
        start++;
        if (*start == '"' || *start == '\'') {
          start++;
          end = strchr(start, *start);
        } else {
          end = strchr(start, '\n');
        }

        if (end) {
          *end = '\0';
          fclose(fp);
          return strdup(start);
        }
      }
    }
  }

  fclose(fp);
  return NULL;
}
const char *get_distro_icon(const char *name) {
  for (size_t i = 0; i < DISTROS_LEN; i++) {
    if (strcmp(distros[i][NAME], name) == 0) {
      return distros[i][ICON];
    }
  }
  return "?";
}

static int width_logo(Bar *bar, BarArg *a) {
  for (size_t i = 0; i < DISTROS_LEN; i++) {
    icon = (char *)get_distro_icon(get_distro_id());
  }
  return TEXTW(icon);
}

static int draw_logo(Bar *bar, BarArg *a) {
  int x = a->x;
  int w = width_logo(bar, a);

  drw_setscheme(drw, scheme[SchemeNorm]);
  drw_text(drw, x, 0, w, bh, lrpad / 2, icon, 0, False);

  return 1;
}

static int click_logo(Bar *bar, Arg *arg, BarArg *a) { return ClkLogo; }
