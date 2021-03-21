/* See LICENSE file for copyright and license details. */

#include <time.h>
#include <X11/XF86keysym.h>

/* appearance */
static const unsigned int borderpx  = 0;        /* border pixel of windows */
static const unsigned int gappx     = 10;	/* gaps between windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const unsigned int systraypinning = 1;   /* 0: sloppy systray follows selected monitor, >0: pin systray to monitor X */
static const unsigned int systrayspacing = 2;   /* systray spacing */
static const int systraypinningfailfirst = 1;   /* 1: if pinning fails, display systray on the first monitor, False: display systray on the last monitor*/
static const int showsystray        = 1;     /* 0 means no systray */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const int user_bh            = 25;       /* 0 means that dwm will calculate bar height, >= 1 means dwm will user_bh as bar height */
static const int vertpad            = 10;       /* vertical padding of bar */
static const int sidepad            = 10;       /* horizontal padding of bar */
static const char *fonts[]          = { "Ubuntu Mono Nerd Font:size=9" };
static const char dmenufont[]       = "Ubuntu Mono Nerd Font:size=9";
static const char col_gray1[]       = "#210142"; //dark violet
static const char col_gray2[]       = "#38096B"; //violet
static const char col_gray3[]       = "#FFFFFF";
static const char col_gray4[]       = "#00E0D1"; //light blue
static const char col_main[]        = "#38096B";
static const char col_bg[]          = "#000000";
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_gray3, col_gray1, col_gray2 },
	[SchemeSel]  = { col_gray4, col_main,  col_main  },
};

/* tagging */
static const char *tags[] = { "", "", "", ""};

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   monitor */
	{ "Gimp",     NULL,       NULL,       0,            0,           -1 },
	{ "Java",     "java",     "Eclipse",  0,            0,           -1 }, // eclipse splash screen
};

/* layout(s) */
static const float mfact     = 0.65; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[ T ]",      tile },    /* first entry is default */
	{ "[ F ]",      NULL },    /* no layout function means floating behavior */
	{ "[ M ]",      monocle },
};

/* key definitions */
#define MODKEY Mod4Mask
#define ALT Mod1Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* commands */
void
spawncmd(const char *cmd[])
{
	const Arg arg = {.v = cmd };
	spawn(&arg);
}

static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[]  = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_main, "-sf", col_gray4, NULL };
static const char *termcmd[]   = { "termite", NULL };
static const char *qpdfcmd[]   = { "qpdfview", NULL };
static const char *bravecmd[] = { "brave", NULL };
static const char *roficmd[] = { "rofi", "-show", "drun", NULL };

static const char *screenshotroot[] = { "bash", "-c", "mkdir -p /tmp/screenshot && scrot /tmp/screenshot/%Y-%m-%d-%H-%M-%S.png", NULL };
static const char *screenshotselection[] = { "bash", "-c", "sleep 0.2 && mkdir -p /tmp/screenshot && scrot /tmp/screenshot/%Y-%m-%d-%H-%M-%S.png --select", NULL };

static const char *upvol[]      = { "amixer", "set", "Master", "1%+", NULL };
static const char *downvol[]    = { "amixer", "set", "Master", "1%-", NULL };
static const char *mute[]       = { "amixer", "set", "Master", "toggle", NULL };

static const char *brightup[]   = { "sudo", "xbacklight", "-inc", "10", NULL};
static const char *brightdown[] = { "sudo", "xbacklight", "-dec", "10", NULL};

static const char *lockscreen[] = { "lock-screen", NULL };

static const char *playerplay[] = { "playerctl", "play-pause", NULL };
static const char *playernext[] = { "playerctl", "next", NULL };
static const char *playerprev[] = { "playerctl", "previous", NULL };
static const char *playerstop[] = { "playerctl", "stop", NULL };

static Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_p,      spawn,          {.v = dmenucmd } },
	{ MODKEY|ShiftMask,             XK_Return, spawn,          {.v = termcmd } },
	{ MODKEY|ALT,                   XK_p,      spawn,          {.v = qpdfcmd } },
	{ MODKEY,                       XK_g,      spawn,          {.v = bravecmd } },
	{ MODKEY|ControlMask,           XK_s,      spawn,          {.v = screenshotroot} },
	{ MODKEY|ShiftMask,             XK_s,      spawn,          {.v = screenshotselection} },
	{ MODKEY|ShiftMask,             XK_q,      quit,           {0} },
	{ MODKEY|ControlMask,           XK_l,      spawn,          {.v = lockscreen } },
	{ MODKEY,                       XK_e,      spawn,          {.v = roficmd } },

	{ 0,            XF86XK_AudioRaiseVolume,   spawn,          {.v = upvol } },
	{ 0,            XF86XK_AudioLowerVolume,   spawn,          {.v = downvol } },
	{ 0,            XF86XK_AudioMute,          spawn,          {.v = mute } },
   	{ 0,            XF86XK_MonBrightnessUp,    spawn,          {.v = brightup } },
    	{ 0,            XF86XK_MonBrightnessUp,    spawn,          {.v = brightdown } },
	{ 0,            XF86XK_AudioPlay,          spawn,          {.v = playerplay } },
	{ 0,            XF86XK_AudioNext,          spawn,          {.v = playernext } },
	{ 0,            XF86XK_AudioPrev,          spawn,          {.v = playerprev } },
	{ 0,            XF86XK_AudioStop,          spawn,          {.v = playerstop } },

	{ MODKEY,                       XK_b,      togglebar,      {0} },
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_d,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY,                       XK_Return, zoom,           {0} },
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY|ShiftMask,             XK_c,      killclient,     {0} },
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_f,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_space,  setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
	{ MODKEY,                       XK_minus,  setgaps,        {.i = -1 } },
	{ MODKEY,                       XK_equal,  setgaps,        {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_equal,  setgaps,        {.i = 0  } },
	{ MODKEY|ControlMask,           XK_Left,   shiftview,      {.i = -1 } },
	{ MODKEY|ControlMask,           XK_Right,  shiftview,      {.i = +1 } },

	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
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

// vim: set noexpandtab :
