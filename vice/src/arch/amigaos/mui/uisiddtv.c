/*
 * uisid.c
 *
 * Written by
 *  Mathias Roslund <vice.emu@amidog.se>
 *
 * This file is part of VICE, the Versatile Commodore Emulator.
 * See README for copyright notice.
 *
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
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
 *  02111-1307  USA.
 *
 */

#include "vice.h"
#ifdef AMIGA_M68K
#define _INLINE_MUIMASTER_H
#endif
#include "mui.h"

#include "sid.h"
#include "machine.h"
#include "ui.h"
#include "uisiddtv.h"
#include "intl.h"
#include "translate.h"

static char *ui_sid_pages[] =
{
    "General",
#ifdef HAVE_RESID
    "ReSID-DTV/ReSID-fp",
#endif
#ifdef HAVE_CATWEASELMKIII
    "Catweasel MK3",
#endif
#ifdef HAVE_HARDSID
    "HardSID",
#endif
    NULL
};

static char *ui_siddtv_engine_model[] = {
#ifdef HAVE_RESID
    "DTVSID (ReSID)",
#endif
    "6581 (Fast SID)",
    "8580 (Fast SID)",
#ifdef HAVE_RESID
    "6581 (ReSID)",
    "8580 (ReSID)",
    "8580 + digiboost (ReSID)",
#endif
#ifdef HAVE_CATWEASELMKIII
    "Catweasel MK3",
#endif
#ifdef HAVE_HARDSID
    "HardSID",
#endif
#ifdef HAVE_RESID_FP
    "6581R3 4885 (ReSID-fp)",
    "6581R3 0486S (ReSID-fp)",
    "6581R3 3984 (ReSID-fp)",
    "6581R4AR 3789 (ReSID-fp)",
    "6581R3 4485 (ReSID-fp)",
    "6581R4 1986S (ReSID-fp)",
    "8580R5 3691 (ReSID-fp)",
    "8580R5 3691 + digiboost (ReSID-fp)",
    "8580R5 1489 (ReSID-fp)",
    "8580R5 1489 + digiboost (ReSID-fp)",
#endif
    0
};

static const int ui_siddtv_engine_model_values[] = {
#ifdef HAVE_RESID
    SID_RESID_DTVSID,
#endif
    SID_FASTSID_6581,
    SID_FASTSID_8580,
#ifdef HAVE_RESID
    SID_RESID_6581,
    SID_RESID_8580,
    SID_RESID_8580D,
#endif
#ifdef HAVE_CATWEASELMKIII
    SID_CATWEASELMKIII,
#endif
#ifdef HAVE_HARDSID
    SID_HARDSID,
#endif
#ifdef HAVE_RESID_FP
    SID_RESIDFP_6581R3_4885,
    SID_RESIDFP_6581R3_0486S,
    SID_RESIDFP_6581R3_3984,
    SID_RESIDFP_6581R4AR_3789,
    SID_RESIDFP_6581R3_4485,
    SID_RESIDFP_6581R4_1986S,
    SID_RESIDFP_8580R5_3691,
    SID_RESIDFP_8580R5_3691D,
    SID_RESIDFP_8580R5_1489,
    SID_RESIDFP_8580R5_1489D,
#endif
    -1
};

static int ui_sid_samplemethod_translate[] =
{
    IDS_FAST,
    IDS_INTERPOLATING,
    IDS_RESAMPLING,
    IDS_FAST_RESAMPLING,
    0
};

static char *ui_sid_samplemethod[countof(ui_sid_samplemethod_translate)];

static const int ui_sid_samplemethod_values[] =
{
    0,
    1,
    2,
    3,
    -1
};

static int ui_band_range[] = {
    0,
    90
};

static ui_to_from_t ui_to_from[] = {
    { NULL, MUI_TYPE_CYCLE_SID, NULL, ui_siddtv_engine_model, ui_siddtv_engine_model_values, NULL },
    { NULL, MUI_TYPE_CHECK, "SidFilters", NULL, NULL, NULL },
#ifdef HAVE_RESID
    { NULL, MUI_TYPE_CYCLE, "SidResidSampling", ui_sid_samplemethod, ui_sid_samplemethod_values, NULL },
    { NULL, MUI_TYPE_INTEGER, "SidResidPassband", NULL, ui_band_range, NULL },
#endif
    UI_END /* mandatory */
};

static APTR build_gui(void)
{
    return RegisterObject,
             MUIA_Register_Titles, ui_sid_pages,
             Child, GroupObject,
               CYCLE(ui_to_from[0].object, translate_text(IDS_SID_ENGINE_MODEL), ui_siddtv_engine_model)
               CHECK(ui_to_from[1].object, translate_text(IDS_SID_FILTERS))
             End,
#if defined(HAVE_RESID) || defined(HAVE_RESID_FP)
             Child, GroupObject,
               CYCLE(ui_to_from[2].object, translate_text(IDS_SAMPLE_METHOD), ui_sid_samplemethod)
               Child, ui_to_from[3].object = StringObject,
                 MUIA_Frame, MUIV_Frame_String,
                 MUIA_FrameTitle, translate_text(IDS_PASSBAND_0_90),
                 MUIA_String_Accept, "0123456789",
                 MUIA_String_MaxLen, 3,
               End,
             End,
#endif
#ifdef HAVE_CATWEASELMKIII
             Child, GroupObject,
               Child, CLabel(translate_text(IDS_NOT_IMPLEMENTED_YET)),
             End,
#endif
#ifdef HAVE_HARDSID
             Child, GroupObject,
               Child, CLabel(translate_text(IDS_NOT_IMPLEMENTED_YET)),
             End,
#endif
           End;
}

void ui_siddtv_settings_dialog(void)
{
    intl_convert_mui_table(ui_sid_samplemethod_translate, ui_sid_samplemethod);
    mui_show_dialog(build_gui(), translate_text(IDS_SID_SETTINGS), ui_to_from);
}
