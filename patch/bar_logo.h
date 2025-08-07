enum { ICON, NAME, DISTRO_COUNT };
#define DISTROS_LEN (sizeof(distros) / sizeof(distros[0]))

static char *icon;
static const char *distros[][DISTRO_COUNT] = {
    {"󰣇", "arch"},   {"", "ubuntu"},     {"", "debian"},
    {"", "fedora"},  {"", "opensuse"},   {"", "centos"},
    {"", "gentoo"},  {"", "slackware"},  {"", "alpine"},
    {"", "manjaro"}, {"", "mint"},       {"", "pop"},
    {"", "kali"},    {"", "elementary"}, {"", "redhat"},
    {"", "void"},    {"", "nixos"},      {"", "raspbian"},
    {"", "mx"},      {"", "zorin"}};

static int width_logo(Bar *bar, BarArg *a);
static int draw_logo(Bar *bar, BarArg *a);
static int click_logo(Bar *bar, Arg *arg, BarArg *a);
static char *get_distro_id(void);
