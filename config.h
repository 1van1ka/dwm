#include <X11/X.h>
#include <X11/XF86keysym.h>

static const unsigned int borderpx       = 3;   /* border pixel of windows */
static const unsigned int cornerrad      = 8;
static const unsigned int border         = 0;   /* always display border: 1, auto display: 0 */
static const unsigned int snap           = 30;  /* snap pixel */
static const unsigned int gappih         = 10;  /* horiz inner gap between windows */
static const unsigned int gappiv         = 10;  /* vert inner gap between windows */
static const unsigned int gappoh         = 4;  /* horiz outer gap between windows and screen edge */
static const unsigned int gappov         = 5;  /* vert outer gap between windows and screen edge */

/* bar settings */
static const int smartgaps_fact          = 0;   /* gap factor when there is only one client; 0 = no gaps, 3 = 3x outer gaps */
static const int showbar                 = 1;   /* 0 means no bar */
static const int topbar                  = 1;   /* 0 means bottom bar */
static const int topline                 = 1;   /* 0 means bottom line active window */
static const int padding_y_bar           = 0;
static const int barwidth                = 0;
static const int barheight               = 24;
static const int lineheight              = 4;

static const unsigned int systrayspacing = 3;   /* systray spacing */
static const int showsystray             = 1;   /* 0 means no systray */

/* layout(s) */
static const float mfact                 = 0.55;  /* factor of master area size [0.05..0.95] */
static const int nmaster                 = 1;    /* number of clients in master area */
static const int resizehints             = 0;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen          = 0;    /* 1 will force focus on the fullscreen window */

static int tagindicatortype              = INDICATOR_TOP_LEFT_SQUARE;
static int tiledindicatortype            = INDICATOR_NONE;
static int floatindicatortype            = INDICATOR_TOP_LEFT_SQUARE;

static const char *fonts[] = { 
  "JetBrains Mono:size=12:style=ExtraBold",
  "Hack Nerd Font:size=12",
  "Noto Color Emoji:size=12",
};

static char *colors[][ColCount] = {
  /*                       fg                bg                border                float */
  [SchemeNorm]         = { "#fff8e6",        "#000000",        "#000000",            "#000000" },
  [SchemeSel]          = { "#fff8e6",        "#8f0000",        "#8f0000",            "#8f0000" },
  [SchemeTitleNorm]    = { "#fff8e6",        "#000000",        "#000000",            "#000000" },
  [SchemeTitleSel]     = { "#fff8e6",        "#8f0000",        "#8f0000",            "#8f0000" },
  [SchemeTagsNorm]     = { "#fff8e6",        "#000000",        "#000000",            "#000000" },
  [SchemeTagsSel]      = { "#fff8e6",        "#8f0000",        "#8f0000",            "#8f0000" },
  [SchemeHidNorm]      = { "#005577",        "#000000",        "#000000",            "#000000" },
  [SchemeHidSel]       = { "#227799",        "#000000",        "#000000",            "#000000" },
  [SchemeUrg]          = { "#aa0000",        "#000000",        "#ff0000",            "#000000" },
};

static char *tagicons[][NUMTAGS] = {
	[DEFAULT_TAGS]        = { "1", "2", "3", "4", "5", "6", "7", "8", "9"},
	[ALTERNATIVE_TAGS]    = { "A", "B", "C", "D", "E", "F", "G", "H", "I" },
};

static const Rule rules[] = {
	RULE(.wintype = WTYPE "DIALOG", .isfloating = 1)
	RULE(.wintype = WTYPE "UTILITY", .isfloating = 1)
	RULE(.wintype = WTYPE "TOOLBAR", .isfloating = 1)
	RULE(.wintype = WTYPE "SPLASH", .isfloating = 1)
	RULE(.class = "feh", .isfloating = 1, .iscentered = 1)
	RULE(.class = "Viewnior", .isfloating = 1, .iscentered = 1)
	RULE(.class = "Pcmanfm", .isfloating = 1, .iscentered = 1, .height = 400, .width = 900)
	RULE(.class = "pavucontrol", .isfloating = 1, .width = 750, .height = 400)
	RULE(.class = "firefox", .tags = 1 << 0)
	RULE(.class = "zen", .tags = 1 << 0)
	RULE(.class = "Code", .tags = 1 << 6)
	RULE(.class = "Atril", .tags = 1 << 5)
	RULE(.class = "TelegramDesktop", .tags = 1 << 3)
	RULE(.class = "discord", .tags = 1 << 3)
  RULE(.class = "steam", .tags = 1 << 2)
	RULE(.class = "Blueman-manager", .isfloating = 1)
  RULE(.class = "mpv", .isfloating = 1, .iscentered = 1, .tags = 1 << 4, .height = 800, .width = 1300)
  RULE(.class = "resources", .isfloating = 1, .iscentered = 1, .height = 800, .width = 1400)
  RULE(.class = "spterm",	.tags = SPTAG(0),	.isfloating = 1, .height = 500, .width = 1000, .y = 100)
  RULE(.class = "KeePassXC", .isfloating = 1, .iscentered = 1, .height = 700, .width = 1000)
  // RULE(.class = "Controlcenter", .isfloating = 1, .x = 2000, .y = 1)
};
 
static const BarRule barrules[] = {
	/* monitor   bar    alignment         widthfunc                 drawfunc                clickfunc                hoverfunc                name */
	{ -1,        0,     BAR_ALIGN_LEFT,   width_tags,               draw_tags,              click_tags,              hover_tags,              "tags" },
  { -1,        0,     BAR_ALIGN_LEFT,   width_logo,               draw_logo,              click_logo,              NULL,                    "logo"},
	{ -1,        0,     BAR_ALIGN_LEFT,   width_ltsymbol,           draw_ltsymbol,          click_ltsymbol,          NULL,                    "layout" },
	{ -1,        0,     BAR_ALIGN_RIGHT,  width_status,             draw_status,            click_status,            NULL,                    "status" },
	{ 'A',       0,     BAR_ALIGN_RIGHT,  width_systray,            draw_systray,           click_systray,           NULL,                    "systray" },
	{ -1,        0,     BAR_ALIGN_NONE,   width_awesomebar,         draw_awesomebar,        click_awesomebar,        NULL,                    "awesomebar" },
};

static const Layout layouts[] = {
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
	{ "|M|",      centeredmaster }
};

static const char *ipcsockpath = "/tmp/dwm.sock";
static IPCCommand ipccommands[] = {
	IPCCOMMAND( focusmon, 1, {ARG_TYPE_SINT} ),
	IPCCOMMAND( focusstack, 1, {ARG_TYPE_SINT} ),
	IPCCOMMAND( incnmaster, 1, {ARG_TYPE_SINT} ),
	IPCCOMMAND( killclient, 1, {ARG_TYPE_SINT} ),
	IPCCOMMAND( quit, 1, {ARG_TYPE_SINT} ),
	IPCCOMMAND( setlayoutsafe, 1, {ARG_TYPE_PTR} ),
	IPCCOMMAND( setmfact, 1, {ARG_TYPE_FLOAT} ),
	IPCCOMMAND( setstatus, 1, {ARG_TYPE_STR} ),
	IPCCOMMAND( tag, 1, {ARG_TYPE_UINT} ),
	IPCCOMMAND( tagmon, 1, {ARG_TYPE_UINT} ),
	IPCCOMMAND( togglebar, 1, {ARG_TYPE_NONE} )
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|Mod1Mask,              KEY,      toggletag,      {.ui = 1 << TAG} },

#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
#define TERMINAL "alacritty"
static const char *dmenucmd[] = { "dmenu_run", NULL };
static const char *spcmd1[] = { TERMINAL, "--class", "spterm", NULL };

static Sp scratchpads[] = {
	/* name          cmd  */
	{"spterm",      spcmd1},
};


static const Key keys[] = {
  /* modifier                     key            function                argument */
  { MODKEY,                       XK_Escape,     spawn,                  {.v = (const char*[]){ TERMINAL, "--class", "resources", "-e", "btop", NULL } } },
  { MODKEY|Mod1Mask,              XK_0,          togglescratch,          {.ui = 0 } },
	{ Mod1Mask|ShiftMask,           XK_space,      spawn,                  {.v = dmenucmd } },
	{ Mod1Mask,                     XK_space,      spawn,                  {.v = (const char*[]){ "dmenu_drun", NULL } } },
	{ MODKEY|ShiftMask,             XK_Return,     spawn,                  {.v = (const char*[]){ TERMINAL, NULL } } },
  { MODKEY,                       XK_a,          spawn,                  {.v = (const char*[]){ "sh", "-c", "controlcenter &" } } },
	{ MODKEY,                       XK_b,          togglebar,              {0} },
	{ MODKEY,                       XK_j,          focusstack,             {.i = +1 } },
	{ MODKEY,                       XK_k,          focusstack,             {.i = -1 } },
	{ MODKEY,                       XK_i,          incnmaster,             {.i = +1 } },
	{ MODKEY,                       XK_d,          incnmaster,             {.i = -1 } },
	{ MODKEY,                       XK_h,          setmfact,               {.f = -0.01} },
	{ MODKEY,                       XK_l,          setmfact,               {.f = +0.01} },
	{ MODKEY,                       XK_0,          togglegaps,             {0} },
	{ MODKEY,                       XK_Tab,        view,                   {0} },
	{ MODKEY|Mod1Mask,              XK_Tab,        focusviadmenu,          {0} },
	{ MODKEY|ControlMask,           XK_z,          showhideclient,         {0} },
	{ MODKEY|ControlMask,           XK_q,          killclient,             {0} },
	{ MODKEY|ShiftMask,             XK_r,          quit,                   {1} },
	{ MODKEY,                       XK_t,          setlayout,              {.v = &layouts[0]} },
	{ MODKEY|ShiftMask,             XK_f,          setlayout,              {.v = &layouts[1]} },
	{ MODKEY,                       XK_m,          setlayout,              {.v = &layouts[2]} },
	{ MODKEY|ShiftMask,             XK_t,          setlayout,              {.v = &layouts[3]} },
	{ MODKEY,                       XK_f,          togglefullscr,          {0} },
	{ MODKEY,                       XK_Return,     zoom,                   {0} },
	{ MODKEY|ShiftMask,             XK_v,          togglefloating,         {0} },
	{ MODKEY,                 			XK_e,          spawn,           		   {.v = (const char*[]){ "pcmanfm", NULL } } },
	{ MODKEY,                 			XK_o,          spawn,           		   {.v = (const char*[]){ "pavucontrol", NULL } } },
	{ MODKEY,             		    	XK_c,          spawn,           		   {.v = (const char*[]){ "clipmenu", NULL } } },
	{ MODKEY,                       XK_comma,      focusmon,               {.i = -1 } },
	{ MODKEY,                       XK_period,     focusmon,               {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,      tagmon,                 {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period,     tagmon,                 {.i = +1 } },
	{ MODKEY|ControlMask,           XK_c,          movecenter,             {0} },

	{ MODKEY|ShiftMask,             XK_q,          moveplace,              {.ui = WIN_NW }},
	{ MODKEY|ShiftMask,             XK_w,          moveplace,              {.ui = WIN_N  }},
	{ MODKEY|ShiftMask,             XK_e,          moveplace,              {.ui = WIN_NE }},
	{ MODKEY|ShiftMask,             XK_a,          moveplace,              {.ui = WIN_W  }},
	{ MODKEY|ShiftMask,             XK_s,          moveplace,              {.ui = WIN_C  }},
	{ MODKEY|ShiftMask,             XK_d,          moveplace,              {.ui = WIN_E  }},
	{ MODKEY|ShiftMask,             XK_z,          moveplace,              {.ui = WIN_SW }},
	{ MODKEY|ShiftMask,             XK_x,          moveplace,              {.ui = WIN_S  }},
	{ MODKEY|ShiftMask,             XK_c,          moveplace,              {.ui = WIN_SE }},
	
	{ MODKEY,             			    XK_n,          spawn,         		     SHCMD("dunstctl close-all; if [[ $(dunstctl get-pause-level) -eq 0 ]]; then notify-send -a notifier 'notifications will be disabled' -t 1000; sleep 1s; dunstctl set-paused toggle; else dunstctl set-paused toggle; notify-send -a notifier 'notifications enabled' -t 1000; fi; pkill -RTMIN+3 dwmblocks") },
	{ MODKEY|Mod1Mask,     			    XK_n,          spawn,         		     SHCMD("dunstctl close-all") },
	{ MODKEY|ShiftMask,             XK_n,          spawn,         		     SHCMD("dunstctl history-pop") },
	{ MODKEY|Mod1Mask,     			    XK_w,          spawn,         		     SHCMD("wifi") },
	{ MODKEY|Mod1Mask,              XK_q,          spawn,         	       SHCMD("powerswitcher") },
	{ Mod1Mask|ControlMask,         XK_Delete,     spawn,                  SHCMD("powermenu") },
	{ MODKEY|ShiftMask|ControlMask|Mod1Mask, XK_q, quit,                   {0} },

	/* laptop key custom */
	{ 0,                            XF86XK_AudioMute,           spawn,     SHCMD("wpctl set-mute @DEFAULT_AUDIO_SINK@ toggle; pkill -RTMIN+1 dwmblocks") },
	{ 0,                            XF86XK_AudioLowerVolume,    spawn,     SHCMD("wpctl set-volume -l 1.2 @DEFAULT_AUDIO_SINK@ 5%-; pkill -RTMIN+1 dwmblocks") },
	{ 0,                            XF86XK_AudioRaiseVolume,    spawn,     SHCMD("wpctl set-volume -l 1.2 @DEFAULT_AUDIO_SINK@ 5%+; pkill -RTMIN+1 dwmblocks") },
	{ ShiftMask,                    XF86XK_AudioLowerVolume,    spawn,     SHCMD("wpctl set-volume -l 1.2 @DEFAULT_AUDIO_SINK@ 1%-; pkill -RTMIN+1 dwmblocks") },
	{ ShiftMask,                    XF86XK_AudioRaiseVolume,    spawn,     SHCMD("wpctl set-volume -l 1.2 @DEFAULT_AUDIO_SINK@ 1%+; pkill -RTMIN+1 dwmblocks") },
	{ 0,                            XF86XK_AudioPlay,           spawn,     SHCMD("playerctl play") },
	{ 0,                            XF86XK_AudioPause,          spawn,     SHCMD("playerctl -a pause") },
	{ 0,                            XF86XK_MonBrightnessUp,     spawn,     SHCMD("brightnessctl --class=\"backlight\" s 5%+") },
	{ 0,                            XF86XK_MonBrightnessDown,   spawn,     SHCMD("brightnessctl --class=\"backlight\" --min-value=1 s 5%-") },
	{ ShiftMask,                    XF86XK_MonBrightnessUp,     spawn,     SHCMD("brightnessctl --class=\"backlight\" s 1%+") },
	{ ShiftMask,                    XF86XK_MonBrightnessDown,   spawn,     SHCMD("brightnessctl --class=\"backlight\" --min-value=1 s 1%-") },
	{ 0,                            XK_Print,                   spawn,     SHCMD("scrot -F Pictures/Screenshots/\"$(date +%Y-%m-%d_%H-%M-%S)\".png -s") },
	{ ShiftMask,                    XK_Print,                   spawn,     SHCMD("scrot -F Pictures/Screenshots/\"$(date +%Y-%m-%d_%H-%M-%S)\".png") },
	{ ControlMask|ShiftMask,        XK_Print,                   spawn,     SHCMD("scrot - | xclip -selection clipboard -t image/png") },
	{ ControlMask,                  XK_Print,                   spawn,     SHCMD("scrot -s - | xclip -selection clipboard -t image/png") },
	// { 0,                            XK_ISO_Next_Group,          spawn,     SHCMD("pkill -RTMIN+10 dwmblocks && ~/.config/dwm/dwm/scripts/layout") },
	{ 0,                            XK_ISO_Next_Group,          spawn,     SHCMD("pkill -RTMIN+10 dwmblocks") },

	TAGKEYS(                        XK_1,                                  0)
	TAGKEYS(                        XK_2,                                  1)
	TAGKEYS(                        XK_3,                                  2)
	TAGKEYS(                        XK_4,                                  3)
	TAGKEYS(                        XK_5,                                  4)
	TAGKEYS(                        XK_6,                                  5)
	TAGKEYS(                        XK_7,                                  6)
	TAGKEYS(                        XK_8,                                  7)
	TAGKEYS(                        XK_9,                                  8)
};

static const Button buttons[] = {
	/* click                event mask           button          function        argument */
	{ ClkLogo,              0,                   Button1,        spawn,          {.v = (const char*[]){ "dmenu_drun", NULL } } },
	{ ClkLogo,              0,                   Button3,        spawn,          {.v = (const char*[]){ "powermenu" } } },
	{ ClkLogo,              MODKEY,              Button1,        spawn,          {.v = (const char*[]){ TERMINAL, NULL } } },
	{ ClkLtSymbol,          0,                   Button1,        spawn,          {.v = (const char*[]){ "controlcenter" } } },

	{ ClkWinTitle,          0,                   Button1,        togglewin,      {0} },
	{ ClkWinTitle,          0,                   Button2,        killclientbar,  {0} },
	{ ClkWinTitle,          0,                   Button3,        showhideclient, {0} },

	{ ClkClientWin,         MODKEY,              Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,              Button2,        togglefloating, {0} },
  { ClkClientWin,         MODKEY,              Button3,        resizemouse,    {0} },

	{ ClkTagBar,            0,                   Button1,        view,           {0} },
	{ ClkTagBar,            0,                   Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,              Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,              Button3,        toggletag,      {0} },
};
