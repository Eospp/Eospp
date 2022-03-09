#include <algorithm.hpp>
#include <util/string.hpp>

namespace eospp::util {

using namespace estd;

void *memcpy(void *dest, const void *src, estd::size_t n) {
    if (!dest || !src || n == 0) return dest;

    if (dest == src) return static_cast<char *>(dest) + n;

    char *dest8 = reinterpret_cast<char *>(dest);
    const char *src8 = reinterpret_cast<const char *>(src);

    if (n >= 8) {
        uint64_t *dest64 = reinterpret_cast<uint64_t *>(dest);
        const uint64_t *src64 = reinterpret_cast<const uint64_t *>(src);

        size_t loop = n / 8;

        for (size_t i = 0; i < loop; i++) {
            dest64[i] = src64[i];
        }

        dest8 += loop * 8;
        src8 += loop * 8;
        n -= loop * 8;
    }

    for (size_t i = 0; i < n; i++) {
        dest8[i] = src8[i];
    }

    return dest8 + n;
}

void *memmove(void *dest, const void *src, estd::size_t n) {
    if (!dest || !src || n == 0) return dest;

    if (dest == src) return static_cast<char *>(dest) + n;

    uint8_t *dest8 = reinterpret_cast<uint8_t *>(dest);
    const uint8_t *src8 = reinterpret_cast<const uint8_t *>(src);
    // no overlap
    if (dest8 < src8) {
        return memcpy(dest8, src8, n);
    }
    // Reverse copying

    uint64_t *dest64 = reinterpret_cast<uint64_t *>(dest8 + n - 1);
    const uint64_t *src64 = reinterpret_cast<const uint64_t *>(src8 + n - 1);

    if (n >= 8) {
        size_t loop = n / 8;

        for (size_t i = 0; i < loop; i++) {
            *(dest64 - i - 1) = *(src64 - i - 1);
        }
        n -= loop * 8;
    }
    dest8 += n - 1;
    src8 += n - 1;

    for (size_t i = 0; i < n; i++) {
        *(dest8 - i) = *(src8 - i);
    }

    return dest64;
}

char *strcpy(char *dest, const char *src) {
    if (!dest || !src) return dest;

    if (dest == src) return dest;

    char *str = dest;

    do {
        *(dest++) = *(src++);
    } while (*(dest - 1) != '\0');

    return str;
}


estd::size_t strlen(const char *str) {
    estd::size_t n = 0;
    while (*(str++) != '\0') n++;
    return n;
}


char *sprintf_num(char *buf,
                  char *last,
                  estd::uint64_t ui64,
                  estd::u_char zero,
                  estd::uintptr_t hexadecimal,
                  estd::uintptr_t width) {
    u_char *p, temp[20 + 1];
    /*
     * we need temp[NGX_INT64_LEN] only,
     * but icc issues the warning
     */
    size_t len;
    uint32_t ui32;
    static u_char hex[] = "0123456789abcdef";
    static u_char HEX[] = "0123456789ABCDEF";

    p = temp + 20;

    if (hexadecimal == 0) {
        if (ui64 <= (uint64_t)0xffffffff) {
            /*
             * To divide 64-bit numbers and to find remainders
             * on the x86 platform gcc and icc call the libc functions
             * [u]divdi3() and [u]moddi3(), they call another function
             * in its turn.  On FreeBSD it is the qdivrem() function,
             * its source code is about 170 lines of the code.
             * The glibc counterpart is about 150 lines of the code.
             *
             * For 32-bit numbers and some divisors gcc and icc use
             * a inlined multiplication and shifts.  For example,
             * unsigned "i32 / 10" is compiled to
             *
             *     (i32 * 0xCCCCCCCD) >> 35
             */

            ui32 = (uint32_t)ui64;

            do {
                *--p = (u_char)(ui32 % 10 + '0');
            } while (ui32 /= 10);

        } else {
            do {
                *--p = (u_char)(ui64 % 10 + '0');
            } while (ui64 /= 10);
        }

    } else if (hexadecimal == 1) {
        do {
            /* the "(uint32_t)" cast disables the BCC's warning */
            *--p = hex[(uint32_t)(ui64 & 0xf)];

        } while (ui64 >>= 4);

    } else { /* hexadecimal == 2 */

        do {
            /* the "(uint32_t)" cast disables the BCC's warning */
            *--p = HEX[(uint32_t)(ui64 & 0xf)];

        } while (ui64 >>= 4);
    }

    /* zero or space padding */

    len = (temp + 20) - p;

    while (len++ < width && buf < last) {
        *buf++ = zero;
    }

    /* number safe copy */

    len = (temp + 20) - p;

    if (buf + len > last) {
        len = last - buf;
    }

    return static_cast<char *>(memcpy(buf, p, len));
}


char *vslprintf(char *buf, char *last, const char *fmt, va_list args) {
    u_char *p, zero;
    int d;
    double f;
    size_t len, slen;
    int64_t i64;
    uint64_t ui64, frac;
    uint32_t width, sign, hex, max_width, frac_width, scale, n;

    while (*fmt && buf < last) {
        /*
         * "buf < last" means that we could copy at least one character:
         * the plain character, "%%", "%c", and minus without the checking
         */

        if (*fmt == '%') {
            i64 = 0;
            ui64 = 0;

            zero = (u_char)((*++fmt == '0') ? '0' : ' ');
            width = 0;
            sign = 1;
            hex = 0;
            max_width = 0;
            frac_width = 0;
            slen = (size_t)-1;

            while (*fmt >= '0' && *fmt <= '9') {
                width = width * 10 + *fmt++ - '0';
            }


            for (;;) {
                switch (*fmt) {
                    case 'u':
                        sign = 0;
                        fmt++;
                        continue;

                    case 'm':
                        max_width = 1;
                        fmt++;
                        continue;

                    case 'X':
                        hex = 2;
                        sign = 0;
                        fmt++;
                        continue;

                    case 'x':
                        hex = 1;
                        sign = 0;
                        fmt++;
                        continue;

                    case '.':
                        fmt++;

                        while (*fmt >= '0' && *fmt <= '9') {
                            frac_width = frac_width * 10 + *fmt++ - '0';
                        }

                        break;

                    case '*':
                        slen = va_arg(args, size_t);
                        fmt++;
                        continue;

                    default:
                        break;
                }

                break;
            }


            switch (*fmt) {
                case 's':
                    p = va_arg(args, u_char *);

                    if (slen == (size_t)-1) {
                        while (*p && buf < last) {
                            *buf++ = *p++;
                        }

                    } else {
                        len = estd::min(((size_t)(last - buf)), slen);
                        buf = (char *)memcpy(buf, p, len);
                    }

                    fmt++;

                    continue;


                case 'z':
                    if (sign) {
                        i64 = (int64_t)va_arg(args, signed int);
                    } else {
                        ui64 = (uint64_t)va_arg(args, size_t);
                    }
                    break;

                case 'i':
                    if (sign) {
                        i64 = (int64_t)va_arg(args, int32_t);
                    } else {
                        ui64 = (uint64_t)va_arg(args, uint32_t);
                    }

                    if (max_width) {
                        width = 20;
                    }

                    break;

                case 'd':
                    if (sign) {
                        i64 = (int64_t)va_arg(args, int);
                    } else {
                        ui64 = (uint64_t)va_arg(args, unsigned int);
                    }
                    break;

                case 'l':
                    if (sign) {
                        i64 = (int64_t)va_arg(args, long);
                    } else {
                        ui64 = (uint64_t)va_arg(args, unsigned long);
                    }
                    break;

                case 'D':
                    if (sign) {
                        i64 = (int64_t)va_arg(args, int32_t);
                    } else {
                        ui64 = (uint64_t)va_arg(args, uint32_t);
                    }
                    break;

                case 'L':
                    if (sign) {
                        i64 = va_arg(args, int64_t);
                    } else {
                        ui64 = va_arg(args, uint64_t);
                    }
                    break;

                case 'f':
                    f = va_arg(args, double);

                    if (f < 0) {
                        *buf++ = '-';
                        f = -f;
                    }

                    ui64 = (int64_t)f;
                    frac = 0;

                    if (frac_width) {
                        scale = 1;
                        for (n = frac_width; n; n--) {
                            scale *= 10;
                        }

                        frac = (uint64_t)((f - (double)ui64) * scale + 0.5);

                        if (frac == scale) {
                            ui64++;
                            frac = 0;
                        }
                    }

                    buf = sprintf_num(buf, last, ui64, zero, 0, width);

                    if (frac_width) {
                        if (buf < last) {
                            *buf++ = '.';
                        }

                        buf = sprintf_num(buf, last, frac, '0', 0, frac_width);
                    }

                    fmt++;

                    continue;

                case 'p':
                    ui64 = (uintptr_t)va_arg(args, void *);
                    hex = 2;
                    sign = 0;
                    zero = '0';
                    width = 8 * 2;
                    break;

                case 'c':
                    d = va_arg(args, int);
                    *buf++ = (u_char)(d & 0xff);
                    fmt++;

                    continue;

                case '%':
                    *buf++ = '%';
                    fmt++;

                    continue;

                default:
                    *buf++ = *fmt++;

                    continue;
            }

            if (sign) {
                if (i64 < 0) {
                    *buf++ = '-';
                    ui64 = (uint64_t)-i64;

                } else {
                    ui64 = (uint64_t)i64;
                }
            }

            buf = sprintf_num(buf, last, ui64, zero, hex, width);

            fmt++;

        } else {
            *buf++ = *fmt++;
        }
    }

    return buf;
}

char *snprintf(char *buf, estd::size_t max, const char *fmt, ...) {
    char *p;
    va_list args;

    va_start(args, fmt);
    p = vslprintf(buf, buf + max, fmt, args);
    va_end(args);
    return p;
}

}   // namespace eospp::util