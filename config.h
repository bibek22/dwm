/* See LICENSE file for copyright and license details. */

/* This for refering to media keys / fn keys */
#include <X11/XF86keysym.h>

/* appearance */
static const unsigned int borderpx  = 2;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const unsigned int systraypinning = 0;   /* 0: sloppy systray follows selected monitor, >0: pin systray to monitor X */
static const unsigned int systrayspacing = 2;   /* systray spacing */
static const int systraypinningfailfirst = 1;   /* 1: if pinning fails, display systray on the first monitor, False: display systray on the last monitor*/
static const int showsystray        = 1;     /* 0 means no systray */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[]          = { "Overpass Nerd Font:size=12" };
static const char dmenufont[]       = "Ubuntu:size=12";
static const char col_gray1[]       = "#222222";
static const char col_gray2[]       = "#444444";
static const char col_gray5[]       = "#cccccc";
static const char col_gray6[]       = "#dddddd";
static const char col_gray3[]       = "#bbbbbb";
static const char col_gray4[]       = "#eeeeee";
static const char col_cyan[]        = "#005577";
static const char col_purple[]      = "#7c6180";
static const char col_bluishpurple[] = "#956fd6";
static const char col_purple_dark[] = "#493f4a";
static const char col_green_dark[]  = "#005577";
static const char col_green[]       = "#005577";
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_gray5, col_purple, col_gray2 },
	[SchemeSel]  = { col_gray6, col_purple_dark,  col_purple  },
};

/* tagging */
static const char *tags[] = { "", "", "", "", "", "", "", "", "" };
static const char *tagsalt[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class				instance    title       tags mask     iscentered  isfloating   monitor */
	{ "Gimp",				NULL,				NULL,				0,						0,					1,					-1 },
	{ "Neomutt",		NULL,				NULL,				1<<2,					0,					1,					-1 },
	{ "firefox",		NULL,				NULL,				1 << 8,				0,					0,					-1 },
	{ "ViberPC",		NULL,				NULL,				1 << 7,				0,					0,					-1 },
	{ "Telegram",		NULL,				NULL,				1 << 7,				0,					0,					-1 },
	{ "Emacs",			NULL,				NULL,				1 << 2,				0,					0,					-1 },
	{ "Pavucontrol",NULL,				NULL,				0,						1,					1,					-1 },
	{ "Zathura",		NULL,				NULL,				1 << 1,				0,					0,					-1 },
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */
static const int attachbelow = 1;    /* 1 means attach after the currently active window */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
};

/* key definitions */
#define MODKEY Mod4Mask
#define ALTKEY Mod1Mask
#define CAKEYS(KEY,CMD) \
	{ ALTKEY|ControlMask,           KEY,      spawn,           {.v = CMD } },
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "rofi", "-show", "drun", NULL };
static const char *emacs[] = { "emacs" , NULL};
static const char *mutt[] = { "st", "-e", "neomutt", NULL};
/* music */
static const char *music[] = { "st", "-e", "ncmpcpp", NULL};
static const char *mpctoggle[] = { "mpcc", "toggle", NULL};
static const char *mpcnext[] = { "mpcc", "next", NULL};
static const char *mpcprev[] = { "mpcc", "prev", NULL};
static const char *vup[] = { "/usr/local/bin/volume_up", NULL};
static const char *vdown[] = { "/usr/local/bin/volume_down", NULL};
static const char *mute[] = { "/usr/local/bin/mute", NULL};
/* brightness */
static const char *bup[] = { "light", "-A", "1", NULL};
static const char *bdown[] = {  "light", "-U", "1", NULL};
/* screenshots */
static const char *screenshot[] = { "/usr/local/bin/screenshot", NULL };
static const char *partial_screenshot[] = { "/usr/local/bin/screenshot", "partial", NULL };
/* utils  */
static const char *jupyter[] = { "/usr/local/bin/jupyterlauncher", NULL };
static const char *omnidoer[] = { "/usr/local/bin/omnidoer", NULL };
static const char *omnilauncher[] = { "/usr/local/bin/omnilauncher", NULL };

static const char *slock[] = { "slock", NULL };
static const char *firefox[] = { "firefox", NULL };
static const char *termcmd[]  = { "st", NULL, NULL };

static Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_p,      spawn,          {.v = dmenucmd } },
	{ MODKEY|ShiftMask,             XK_Return, spawn,          {.v = termcmd } },
	{ MODKEY,                       XK_b,      togglebar,      {0} },
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_d,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY,                       XK_Return, zoom,           {0} },
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY,			XK_w,      killclient,     {0} },
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_f,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_space,  setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
	{ MODKEY|ShiftMask,             XK_f,      togglefullscr,  {0} },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
	{ MODKEY,                       XK_n,      togglealttag,   {0} },
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
	{ MODKEY|ShiftMask,             XK_q,      quit,           {0} },

     /* My shortcuts below  */
	CAKEYS(				XK_g,                      firefox)
	CAKEYS(				XK_l,                      slock)
	CAKEYS(				XK_e,                      emacs)
	CAKEYS(				XK_m,                      mutt)
	CAKEYS(				XK_k,                      music)
	CAKEYS(				XK_j,                      jupyter)
	CAKEYS(				XK_o,                      omnilauncher)

	{ MODKEY,        XK_o,			 spawn,					{.v = omnidoer } },

/* Here's a link to the keys defined in X11
 https://cgit.freedesktop.org/xorg/proto/x11proto/tree/XF86keysym.h */

	{ MODKEY,             XF86XK_RockerDown,    spawn,          {.v = vdown} },
	{ MODKEY,             XF86XK_RockerUp,    spawn,          {.v = vup} },
	{ MODKEY,             XF86XK_AudioPlay,		spawn,          {.v = mpctoggle } },
	{ MODKEY,             XF86XK_AudioPrev,		spawn,          {.v = mpcprev } },
	{ MODKEY,             XF86XK_AudioNext,    spawn,          {.v = mpcnext } },

	{ MODKEY,             XK_F4,      spawn,          {.v = bdown} },
	{ MODKEY,             XK_F5,      spawn,          {.v = bup} },

	{ ControlMask,             XK_F8,			 spawn,          {.v = mpcprev } },
	{ ControlMask,             XK_F9,			 spawn,          {.v = mpctoggle } },
	{ ControlMask,             XK_F10,     spawn,          {.v = mpcnext } },
	{ ControlMask,             XK_F11,     spawn,          {.v = vdown} },
	{ ControlMask,             XK_F12,     spawn,          {.v = vup} },
	{ 0,          XF86XK_AudioMute,		spawn, {.v = mute } },
	{ 0,						 XK_Print,		spawn, {.v = screenshot } },
	{ ControlMask,	 XK_Print,		spawn, {.v = partial_screenshot } },

	{ 0,						 XF86XK_Mail,		spawn, {.v =  mutt} },
	{ 0,						 XF86XK_Start,		spawn, {.v = dmenucmd} },
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

