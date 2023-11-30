/* See LICENSE file for copyright and license details. */

/* appearance */
static const unsigned int borderpx  = 3;        /* border pixel of windows */
static const unsigned int margin    = 10;       /* margin between windows (must be even) */
static const unsigned int snap      = 32;       /* snap pixel */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[]          = { "Source Code Pro Semibold:size=12" };
static const char dmenufont[]       = "Source Code Pro Semibold:size=12";
static const char col_black[]       = "#282828";
static const char col_gray[]        = "#928374";
static const char col_red[]         = "#cc241d";
static const char col_white[]       = "#ebdbb2";
static const char col_yellow[]      = "#d79921";
static const char col_blue[]        = "#458588";
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_white, col_black, col_black },
	[SchemeSel]  = { col_white, col_red, col_red  },
};

/* transparency */
static const char *transparencycmd[]   = {
	[TranspDec] = "transset-df -a --dec",
	[TranspInc] = "transset-df -a --inc",
	[TranspSet] = "transset-df -a",
};

/* tagging */
static const char *tags[] = { "dev", "www", "pdf", "mus", "misc", "comm", "ntua" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class           instance    title       tags mask     isfloating   monitor, transparency */
	{ "Gimp",          NULL,       NULL,       0,            1,           -1,      1  },
	{ "Firefox",       NULL,       NULL,       1 << 8,       0,           -1,      1  },
	{ "Yad",           NULL,       NULL,       0,            1,           -1,      0.95 },
	{ "Open3D",        NULL,       NULL,       0,            1,           -1,      0.95 },
	{ "Qalculate-gtk", NULL,       NULL,       0,            1,           -1,      0.95 },
	{ "Alacritty",     NULL,       NULL,       0,            0,           -1,      0.98 },
	{ "Matplotlib",    NULL,       NULL,       0,            1,           -1,      1 },
};

/* layout(s) */
static const float mfact     = 0.6; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "[M]",      monocle },
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ NULL,       NULL }     /* necessary for cyclelayouts() */
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static const char autostart[] = ".local/dwm/autostart"; /* executable bash file to run at autostart (blocking) */
static const char help[] =  ".local/dwm/shortcuts"; /* help script (unblocking) */
static const char monitors[] =  ".local/dwm/monitors";
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_black, "-nf", col_white, "-sb", col_red, "-sf", col_white, NULL };
static const char *editorcmd[] = { "alacritty", "-e", "vim" };
static const char *termcmd[]  = { "alacritty", NULL };
static const char *netcmd[]  = { "alacritty", "-e", "nmtui" };
static const char *brightupcmd[] = { "brightnessctl", "set", "10%+" };
static const char *brightdowncmd[] = { "brightnessctl", "set", "10%-" };
static const char *brightcmd[] = { "brightnessctl", "set", "100%" };
static const char *pausecmd[] = { "playerctl", "play-pause" };
static const char *nextcmd[] = { "playerctl", "next" };
static const char *prevcmd[] = { "playerctl", "previous" };
static const char *mutecmd[] = { "pactl", "set-sink-mute", "@DEFAULT_SINK@", "toggle", NULL };
static const char *volupcmd[] = { "pactl", "set-sink-volume", "@DEFAULT_SINK@", "+5%", NULL };
static const char *voldowncmd[] = { "pactl", "set-sink-volume", "@DEFAULT_SINK@", "-5%", NULL };

static const Key keys[] = {
	/* modifier                     key                     function           argument */
	{ MODKEY,                       XK_Return,               spawn,            {.v = termcmd} },       // -- Open terminal
	{ MODKEY|ShiftMask,             XK_Return,               spawn,            {.v = dmenucmd} },      // Spawn dmenu
	{ MODKEY|ShiftMask,             XK_slash,                spawnlocal,       {.v = help} },          // -- Open help page
	{ MODKEY|ShiftMask,             XK_q,                    quit,             {0} },                  // Close dwm
	{ MODKEY|ShiftMask,             XK_r,                    quit,             {1} },                  // Restart dwm
	{ MODKEY|ShiftMask,             XK_b,                    togglebar,        {0} },                  // Toggle status bar
	{ MODKEY|ShiftMask,             XK_c,                    killclient,       {0} },                  // -- Kill current client
	{ MODKEY,                       XK_j,                    focusstack,       {.i = +1} },            // Focus on next-previous client
	{ MODKEY,                       XK_k,                    focusstack,       {.i = -1} },
	{ MODKEY|ShiftMask,             XK_j,                    shiftclient,      {.i = +1} },            // Move client up-down on stack
	{ MODKEY|ShiftMask,             XK_k,                    shiftclient,      {.i = -1} },
	{ MODKEY|ShiftMask,             XK_m,                    shiftmaster,      {0} },                  // Move client to master
	{ MODKEY,                       XK_n,                    incnmaster,       {.i = +1} },            // Increase-decrease master clients
	{ MODKEY,                       XK_m,                    incnmaster,       {.i = -1} },
	{ MODKEY,                       XK_h,                    setmfact,         {.f = -0.05} },         // Increase-Decrease master percentage
	{ MODKEY,                       XK_l,                    setmfact,         {.f = +0.05} },
	{ MODKEY,                       XK_equal,                setmfact,         {.f = +3.00} },         // Set master to default
	{ ControlMask,                  XK_space,                cyclelayouts,     { .i = +1} },           // -- Change to next layout
	{ ControlMask|ShiftMask,        XK_space,                setlayout,        { .v = &layouts[0]} },  // Change to default layout
	{ MODKEY,                       XK_f,                    togglefloating,   {0} },                  // Toggle floating on current client
	{ MODKEY|ShiftMask,             XK_f,                    togglefullscreen, {0} },                  // Toggle fullscreen on current client
	{ MODKEY,                       XK_Tab,                  view,             {0} },                  // Change to last tag
	{ MODKEY,                       XK_0,                    view,             {.ui = ~0} },           // View all tags
	{ MODKEY|ShiftMask,             XK_0,                    tag,              {.ui = ~0} },           // Move client to all tags
	{ MODKEY,                       XK_comma,                focusmon,         {.i = -1} },            // Focus on next-previous monitor
	{ MODKEY,                       XK_period,               focusmon,         {.i = +1} },
	{ MODKEY|ShiftMask,             XK_comma,                tagmon,           {.i = -1} },            // Move client to next-previous monitor
	{ MODKEY|ShiftMask,             XK_period,               tagmon,           {.i = +1} },
	/*
	{ MODKEY,                       KEY,                                       },                      // Change to tag
	{ MODKEY|ControlMask,           KEY,                                       },                      // Toggle tag
	{ MODKEY|ShiftMask,             KEY,                                       },                      // Move client to tag
	{ MODKEY|ControlMask|ShiftMask, KEY,                                       },                      // Display client (also) to tag
	{ y + u + i + o + p + [ ,       ],                                         },                      // Key can be one of :
	*/
	TAGKEYS(                        XK_y,                                      0)
	TAGKEYS(                        XK_y,                                      0)
	TAGKEYS(                        XK_u,                                      1)
	TAGKEYS(                        XK_i,                                      2)
	TAGKEYS(                        XK_o,                                      3)
	TAGKEYS(                        XK_p,                                      4)
	TAGKEYS(                        XK_bracketleft,                            5)
	TAGKEYS(                        XK_bracketright,                           6)
	{ 0,                            XF86XK_AudioPlay,        spawn,            {.v = pausecmd} },
	{ 0,                            XF86XK_AudioNext,        spawn,            {.v = nextcmd} },
	{ 0,                            XF86XK_AudioPrev,        spawn,            {.v = prevcmd} },
	{ 0,                            XF86XK_AudioMute,        spawn,            {.v = mutecmd} },
	{ 0,                            XF86XK_AudioLowerVolume, spawn,            {.v = voldowncmd} },
	{ 0,                            XF86XK_AudioRaiseVolume, spawn,            {.v = volupcmd} },
};

/* modes */
const Key Launch[] = { // MODKEY, XK_r
	{ MODKEY,                       XK_r,                    spawn,            {.v = dmenucmd} },      // Dmenu
	{ MODKEY,                       XK_b,                    spawn,            SHCMD("firefox") },     // Browser
	{ MODKEY,                       XK_f,                    spawn,            SHCMD("pcmanfm") },     // File browser
	{ MODKEY,                       XK_e,                    spawn,            {.v = editorcmd} },     // Editor
	{ MODKEY,                       XK_p,                    spawn,            SHCMD("evince") },      // PDF viewer
	{ MODKEY,                       XK_m,                    spawn,            SHCMD("pavucontrol") }, // Audio control GUI
	{ MODKEY,                       XK_c,                    spawn,            SHCMD("qalculate-gtk") }, // Calculator
	{ MODKEY,                       XK_s,                    spawn,            SHCMD("flameshot gui") }, // Screenshot tool
	{ MODKEY,                       XK_n,                    spawn,            {.v = netcmd} },        // Network manager
};
const Key Transparency[] = { // MODKEY, XK_b
	{ MODKEY|ShiftMask,            XK_equal,                settransparency,   {.f = +0.05} },         // Increase
	{ MODKEY,                      XK_minus,                settransparency,   {.f = -0.05} },         // Decrease
	{ MODKEY,                      XK_equal,                settransparency,   {0} },                  // Set to default
};
const Key Brightness[] = { // MODKEY, XK_b
	{ MODKEY|ShiftMask,            XK_equal,                spawn,             {.v = brightupcmd} },   // Increase
	{ MODKEY,                      XK_minus,                spawn,             {.v = brightdowncmd}},  // Increase
	{ MODKEY,                      XK_equal,                spawn,             {.v = brightcmd} },     // Increase
};

const Mode modes[] = {
	/* symbol,        mod,    key,  unique, keys,         keys length   */
	{ "launch",       MODKEY, XK_r, 1,      Launch,       LENGTH(Launch) },
	{ "brightness",   MODKEY, XK_b, 0,      Brightness,   LENGTH(Brightness) },
	{ "transparency", MODKEY, XK_t, 0,      Transparency, LENGTH(Transparency) },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static const Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

