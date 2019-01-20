# ---------------------User Definition Area Begin-----------------------------
# PLN_SOURCE: Specify plan source file name whose extension name is .pln.
# PLN_CFLAGS: plncmp options on compiling phase e.g. -c, -v
#
# PAT_SOURCE: Specify pattern source file name whose extension name is .pat.
# PAT_CFLAGS: patcmp options on compiling phase e.g. -x, -p, -eXXX
# PAT_LFLAGS: patcmp options on linking phase e.g -oXXX, -r
# NOCOMPRESS_FLAG => remove comment mark "#" below to turn on the patcmp's "-n" flag.
#
PATH_PAT0     = .\PAT
PLN_SOURCE   = IC_Ctrl.pln RN8213_1S_3380D_0002_B_Version.pln spline.pln 
PLN_TARGET   = RN8213_1S_3380D_0002_B_Version.pin
PLN_CFLAGS   = 
PAT_SOURCE0   =                 $(PATH_PAT0)\Capture_WORD.pat\ 
              $(PATH_PAT0)\GET_CHIP_ID.PAT\ 
              $(PATH_PAT0)\READ_WORD.pat\ 
              $(PATH_PAT0)\reset_swd.PAT\ 
              $(PATH_PAT0)\WIRTE_WORD.pat 

PAT_CFLAGS0   =
PAT_LFLAGS0   = -o.\PAT\RN8213
PAT_TARGET0   =.\PAT\RN8213.ppo
# ---------------------User Definition Area End------------------------------

PAT_OBJECTS_TMP0    = $(PAT_SOURCE0:.pat=.pdt)
PAT_OBJECTS0        = $(PAT_OBJECTS_TMP0:.PAT=.pdt)
# ---------------------------------------------------------------------------
.precious: $(PAT_OBJECTS0) $(PAT_COBJECTS0)

all: $(PLN_TARGET) $(PAT_TARGET0) 

$(PLN_TARGET): $(PLN_SOURCE) .\PAT\RN8213.dec
  @plncmp $(PLN_CFLAGS) $(PLN_SOURCE)

$(PAT_TARGET0):: .\PAT\RN8213.dec
  @patcmp $(PAT_CFLAGS0) $(PAT_LFLAGS0) -f makefile_pat0.lst

$(PAT_TARGET0):: $(PAT_OBJECTS0)
  @patcmp $(PAT_LFLAGS0) -f makefile_pdt0.lst

.pat.pdt :
  @patcmp -c -s $(PAT_CFLAGS0) $<

build:
  @plncmp $(PLN_CFLAGS) $(PLN_SOURCE)
  @patcmp $(PAT_CFLAGS0) $(PAT_LFLAGS0) -f makefile_pat0.lst

clean:
  @touch -c *.pln
  @touch -c $(PATH_PAT0)\Capture_WORD.pat
  @touch -c $(PATH_PAT0)\GET_CHIP_ID.PAT
  @touch -c $(PATH_PAT0)\READ_WORD.pat
  @touch -c $(PATH_PAT0)\reset_swd.PAT
  @touch -c $(PATH_PAT0)\WIRTE_WORD.pat
