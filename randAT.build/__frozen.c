// This provides the frozen (compiled bytecode) files that are included if
// any.
#include <Python.h>

#include "nuitka/constants_blob.h"

// Blob from which modules are unstreamed.
#define stream_data constant_bin

// These modules should be loaded as bytecode. They may e.g. have to be loadable
// during "Py_Initialize" already, or for irrelevance, they are only included
// in this un-optimized form. These are not compiled by Nuitka, and therefore
// are not accelerated at all, merely bundled with the binary or module, so
// that CPython library can start out finding them.

struct frozen_desc {
    char const *name;
    ssize_t start;
    int size;
};

void copyFrozenModulesTo( struct _frozen *destination )
{
    struct frozen_desc frozen_modules[] = {
        { "base64", 7603115, 11780 },
        { "codecs", 7614895, 41401 },
        { "copy_reg", 7656296, 5655 },
        { "encodings", 7661951, -4570 },
        { "encodings.aliases", 7666521, 8812 },
        { "encodings.ascii", 7675333, 2773 },
        { "encodings.base64_codec", 7678106, 4470 },
        { "encodings.big5", 7682576, 2112 },
        { "encodings.big5hkscs", 7684688, 2152 },
        { "encodings.bz2_codec", 7686840, 5570 },
        { "encodings.charmap", 7692410, 4245 },
        { "encodings.cp037", 7696655, 3401 },
        { "encodings.cp1006", 7700056, 3487 },
        { "encodings.cp1026", 7703543, 3415 },
        { "encodings.cp1140", 7706958, 3401 },
        { "encodings.cp1250", 7710359, 3438 },
        { "encodings.cp1251", 7713797, 3435 },
        { "encodings.cp1252", 7717232, 3438 },
        { "encodings.cp1253", 7720670, 3451 },
        { "encodings.cp1254", 7724121, 3440 },
        { "encodings.cp1255", 7727561, 3459 },
        { "encodings.cp1256", 7731020, 3437 },
        { "encodings.cp1257", 7734457, 3445 },
        { "encodings.cp1258", 7737902, 3443 },
        { "encodings.cp424", 7741345, 3431 },
        { "encodings.cp437", 7744776, 8636 },
        { "encodings.cp500", 7753412, 3401 },
        { "encodings.cp720", 7756813, 3498 },
        { "encodings.cp737", 7760311, 8864 },
        { "encodings.cp775", 7769175, 8650 },
        { "encodings.cp850", 7777825, 8383 },
        { "encodings.cp852", 7786208, 8652 },
        { "encodings.cp855", 7794860, 8833 },
        { "encodings.cp856", 7803693, 3463 },
        { "encodings.cp857", 7807156, 8373 },
        { "encodings.cp858", 7815529, 8353 },
        { "encodings.cp860", 7823882, 8619 },
        { "encodings.cp861", 7832501, 8630 },
        { "encodings.cp862", 7841131, 8765 },
        { "encodings.cp863", 7849896, 8630 },
        { "encodings.cp864", 7858526, 8761 },
        { "encodings.cp865", 7867287, 8630 },
        { "encodings.cp866", 7875917, 8865 },
        { "encodings.cp869", 7884782, 8677 },
        { "encodings.cp874", 7893459, 3529 },
        { "encodings.cp875", 7896988, 3398 },
        { "encodings.cp932", 7900386, 2120 },
        { "encodings.cp949", 7902506, 2120 },
        { "encodings.cp950", 7904626, 2120 },
        { "encodings.euc_jis_2004", 7906746, 2176 },
        { "encodings.euc_jisx0213", 7908922, 2176 },
        { "encodings.euc_jp", 7911098, 2128 },
        { "encodings.euc_kr", 7913226, 2128 },
        { "encodings.gb18030", 7915354, 2136 },
        { "encodings.gb2312", 7917490, 2128 },
        { "encodings.gbk", 7919618, 2104 },
        { "encodings.hex_codec", 7921722, 4422 },
        { "encodings.hp_roman8", 7926144, 4684 },
        { "encodings.hz", 7930828, 2096 },
        { "encodings.idna", 7932924, 7096 },
        { "encodings.iso2022_jp", 7940020, 2165 },
        { "encodings.iso2022_jp_1", 7942185, 2181 },
        { "encodings.iso2022_jp_2", 7944366, 2181 },
        { "encodings.iso2022_jp_2004", 7946547, 2205 },
        { "encodings.iso2022_jp_3", 7948752, 2181 },
        { "encodings.iso2022_jp_ext", 7950933, 2197 },
        { "encodings.iso2022_kr", 7953130, 2165 },
        { "encodings.iso8859_1", 7955295, 3440 },
        { "encodings.iso8859_10", 7958735, 3455 },
        { "encodings.iso8859_11", 7962190, 3549 },
        { "encodings.iso8859_13", 7965739, 3458 },
        { "encodings.iso8859_14", 7969197, 3476 },
        { "encodings.iso8859_15", 7972673, 3455 },
        { "encodings.iso8859_16", 7976128, 3457 },
        { "encodings.iso8859_2", 7979585, 3440 },
        { "encodings.iso8859_3", 7983025, 3447 },
        { "encodings.iso8859_4", 7986472, 3440 },
        { "encodings.iso8859_5", 7989912, 3441 },
        { "encodings.iso8859_6", 7993353, 3485 },
        { "encodings.iso8859_7", 7996838, 3448 },
        { "encodings.iso8859_8", 8000286, 3479 },
        { "encodings.iso8859_9", 8003765, 3440 },
        { "encodings.johab", 8007205, 2120 },
        { "encodings.koi8_r", 8009325, 3462 },
        { "encodings.koi8_u", 8012787, 3448 },
        { "encodings.latin_1", 8016235, 2803 },
        { "encodings.mac_arabic", 8019038, 8586 },
        { "encodings.mac_centeuro", 8027624, 3509 },
        { "encodings.mac_croatian", 8031133, 3517 },
        { "encodings.mac_cyrillic", 8034650, 3507 },
        { "encodings.mac_farsi", 8038157, 3421 },
        { "encodings.mac_greek", 8041578, 3461 },
        { "encodings.mac_iceland", 8045039, 3500 },
        { "encodings.mac_latin2", 8048539, 5479 },
        { "encodings.mac_roman", 8054018, 3478 },
        { "encodings.mac_romanian", 8057496, 3518 },
        { "encodings.mac_turkish", 8061014, 3501 },
        { "encodings.palmos", 8064515, 3638 },
        { "encodings.ptcp154", 8068153, 5462 },
        { "encodings.punycode", 8073615, 9176 },
        { "encodings.quopri_codec", 8082791, 4267 },
        { "encodings.raw_unicode_escape", 8087058, 2670 },
        { "encodings.rot_13", 8089728, 4245 },
        { "encodings.shift_jis", 8093973, 2152 },
        { "encodings.shift_jis_2004", 8096125, 2192 },
        { "encodings.shift_jisx0213", 8098317, 2192 },
        { "encodings.string_escape", 8100509, 2529 },
        { "encodings.tis_620", 8103038, 3510 },
        { "encodings.undefined", 8106548, 3161 },
        { "encodings.unicode_escape", 8109709, 2618 },
        { "encodings.unicode_internal", 8112327, 2644 },
        { "encodings.utf_16", 8114971, 6200 },
        { "encodings.utf_16_be", 8121171, 2406 },
        { "encodings.utf_16_le", 8123577, 2406 },
        { "encodings.utf_32", 8125983, 6868 },
        { "encodings.utf_32_be", 8132851, 2299 },
        { "encodings.utf_32_le", 8135150, 2299 },
        { "encodings.utf_7", 8137449, 2299 },
        { "encodings.utf_8", 8139748, 2358 },
        { "encodings.utf_8_sig", 8142106, 6017 },
        { "encodings.uu_codec", 8148123, 5550 },
        { "encodings.zlib_codec", 8153673, 5490 },
        { "functools", 8159163, 7381 },
        { "locale", 8166544, 56680 },
        { "quopri", 8223224, 7116 },
        { "re", 8230340, 14455 },
        { "sre_compile", 8244795, 13301 },
        { "sre_constants", 8258096, 6437 },
        { "sre_parse", 8264533, 21809 },
        { "stringprep", 8286342, 15481 },
        { "struct", 8301823, 281 },
        { "types", 8302104, 3067 },
        { NULL, 0, 0 }
    };

    struct frozen_desc *current = frozen_modules;

    for(;;)
    {
        destination->name = (char *)current->name;
        destination->code = (unsigned char *)&constant_bin[ current->start ];
        destination->size = current->size;

        if (destination->name == NULL) break;

        current += 1;
        destination += 1;
    };
}
