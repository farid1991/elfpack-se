const char default_palette[][4] = {
    { 0, 0, 0, 0 }, /* 0 */
    { 29, 29, 29, 0 }, /* 1 */
    { 35, 6, 9, 0 }, /* 2 */
    { 42, 0, 0, 0 }, /* 3 */
    { 39, 0, 17, 0 }, /* 4 */
    { 29, 0, 35, 0 }, /* 5 */
    { 4, 0, 42, 0 }, /* 6 */
    { 0, 0, 41, 0 }, /* 7 */
    { 0, 2, 31, 0 }, /* 8 */
    { 0, 11, 16, 0 }, /* 9 */
    { 0, 17, 0, 0 }, /* 10 */
    { 0, 20, 0, 0 }, /* 11 */
    { 5, 15, 0, 0 }, /* 12 */
    { 23, 15, 6, 0 }, /* 13 */
    { 0, 0, 0, 0 }, /* 14 */
    { 1, 1, 1, 0 }, /* 15 */
    { 1, 1, 1, 0 }, /* 16 */
    { 47, 47, 47, 0 }, /* 17 */
    { 59, 28, 0, 0 }, /* 18 */
    { 59, 14, 8, 0 }, /* 19 */
    { 60, 0, 32, 0 }, /* 20 */
    { 47, 0, 47, 0 }, /* 21 */
    { 22, 0, 57, 0 }, /* 22 */
    { 0, 10, 54, 0 }, /* 23 */
    { 3, 19, 50, 0 }, /* 24 */
    { 0, 28, 34, 0 }, /* 25 */
    { 0, 37, 0, 0 }, /* 26 */
    { 0, 42, 0, 0 }, /* 27 */
    { 14, 36, 0, 0 }, /* 28 */
    { 34, 32, 0, 0 }, /* 29 */
    { 4, 4, 4, 0 }, /* 30 */
    { 2, 2, 2, 0 }, /* 31 */
    { 2, 2, 2, 0 }, /* 32 */
    { 63, 63, 63, 0 }, /* 33 */
    { 63, 47, 15, 0 }, /* 34 */
    { 63, 37, 23, 0 }, /* 35 */
    { 63, 34, 41, 0 }, /* 36 */
    { 63, 30, 61, 0 }, /* 37 */
    { 45, 29, 63, 0 }, /* 38 */
    { 24, 29, 63, 0 }, /* 39 */
    { 14, 38, 63, 0 }, /* 40 */
    { 15, 47, 60, 0 }, /* 41 */
    { 4, 52, 32, 0 }, /* 42 */
    { 18, 55, 19, 0 }, /* 43 */
    { 38, 62, 22, 0 }, /* 44 */
    { 54, 58, 0, 0 }, /* 45 */
    { 25, 25, 25, 0 }, /* 46 */
    { 3, 3, 3, 0 }, /* 47 */
    { 3, 3, 3, 0 }, /* 48 */
    { 63, 63, 63, 0 }, /* 49 */
    { 63, 57, 42, 0 }, /* 50 */
    { 63, 53, 49, 0 }, /* 51 */
    { 63, 50, 53, 0 }, /* 52 */
    { 63, 49, 63, 0 }, /* 53 */
    { 54, 49, 63, 0 }, /* 54 */
    { 44, 47, 63, 0 }, /* 55 */
    { 42, 54, 63, 0 }, /* 56 */
    { 40, 57, 63, 0 }, /* 57 */
    { 40, 63, 56, 0 }, /* 58 */
    { 47, 60, 42, 0 }, /* 59 */
    { 51, 63, 44, 0 }, /* 60 */
    { 60, 63, 39, 0 }, /* 61 */
    { 55, 55, 55, 0 }, /* 62 */
    { 4, 4, 4, 0 }, /* 63 */
    { 4, 4, 4, 0 }, /* 64 */
    { 0, 0, 0, 0 }, /* 65 */
    { 0, 0, 0, 0 }, /* 66 */
    { 0, 0, 0, 0 }, /* 67 */
    { 0, 0, 0, 0 }, /* 68 */
    { 0, 0, 0, 0 }, /* 69 */
    { 0, 0, 0, 0 }, /* 70 */
    { 0, 0, 0, 0 }, /* 71 */
    { 0, 0, 0, 0 }, /* 72 */
    { 0, 0, 0, 0 }, /* 73 */
    { 0, 0, 0, 0 }, /* 74 */
    { 0, 0, 0, 0 }, /* 75 */
    { 0, 0, 0, 0 }, /* 76 */
    { 0, 0, 0, 0 }, /* 77 */
    { 0, 0, 0, 0 }, /* 78 */
    { 0, 0, 0, 0 }, /* 79 */
    { 0, 0, 0, 0 }, /* 80 */
    { 0, 0, 0, 0 }, /* 81 */
    { 0, 0, 0, 0 }, /* 82 */
    { 0, 0, 0, 0 }, /* 83 */
    { 0, 0, 0, 0 }, /* 84 */
    { 0, 0, 0, 0 }, /* 85 */
    { 0, 0, 0, 0 }, /* 86 */
    { 0, 0, 0, 0 }, /* 87 */
    { 0, 0, 0, 0 }, /* 88 */
    { 0, 0, 0, 0 }, /* 89 */
    { 0, 0, 0, 0 }, /* 90 */
    { 0, 0, 0, 0 }, /* 91 */
    { 0, 0, 0, 0 }, /* 92 */
    { 0, 0, 0, 0 }, /* 93 */
    { 0, 0, 0, 0 }, /* 94 */
    { 0, 0, 0, 0 }, /* 95 */
    { 0, 0, 0, 0 }, /* 96 */
    { 0, 0, 0, 0 }, /* 97 */
    { 0, 0, 0, 0 }, /* 98 */
    { 0, 0, 0, 0 }, /* 99 */
    { 0, 0, 0, 0 }, /* 100 */
    { 0, 0, 0, 0 }, /* 101 */
    { 0, 0, 0, 0 }, /* 102 */
    { 0, 0, 0, 0 }, /* 103 */
    { 0, 0, 0, 0 }, /* 104 */
    { 0, 0, 0, 0 }, /* 105 */
    { 0, 0, 0, 0 }, /* 106 */
    { 0, 0, 0, 0 }, /* 107 */
    { 0, 0, 0, 0 }, /* 108 */
    { 0, 0, 0, 0 }, /* 109 */
    { 0, 0, 0, 0 }, /* 110 */
    { 0, 0, 0, 0 }, /* 111 */
    { 0, 0, 0, 0 }, /* 112 */
    { 0, 0, 0, 0 }, /* 113 */
    { 0, 0, 0, 0 }, /* 114 */
    { 0, 0, 0, 0 }, /* 115 */
    { 0, 0, 0, 0 }, /* 116 */
    { 0, 0, 0, 0 }, /* 117 */
    { 0, 0, 0, 0 }, /* 118 */
    { 0, 0, 0, 0 }, /* 119 */
    { 0, 0, 0, 0 }, /* 120 */
    { 0, 0, 0, 0 }, /* 121 */
    { 0, 0, 0, 0 }, /* 122 */
    { 0, 0, 0, 0 }, /* 123 */
    { 0, 0, 0, 0 }, /* 124 */
    { 0, 0, 0, 0 }, /* 125 */
    { 0, 0, 0, 0 }, /* 126 */
    { 0, 0, 0, 0 }, /* 127 */
    { 0, 0, 0, 0 }, /* 128 */
    { 0, 0, 0, 0 }, /* 129 */
    { 0, 0, 0, 0 }, /* 130 */
    { 0, 0, 0, 0 }, /* 131 */
    { 0, 0, 0, 0 }, /* 132 */
    { 0, 0, 0, 0 }, /* 133 */
    { 0, 0, 0, 0 }, /* 134 */
    { 0, 0, 0, 0 }, /* 135 */
    { 0, 0, 0, 0 }, /* 136 */
    { 0, 0, 0, 0 }, /* 137 */
    { 0, 0, 0, 0 }, /* 138 */
    { 0, 0, 0, 0 }, /* 139 */
    { 0, 0, 0, 0 }, /* 140 */
    { 0, 0, 0, 0 }, /* 141 */
    { 0, 0, 0, 0 }, /* 142 */
    { 0, 0, 0, 0 }, /* 143 */
    { 0, 0, 0, 0 }, /* 144 */
    { 0, 0, 0, 0 }, /* 145 */
    { 0, 0, 0, 0 }, /* 146 */
    { 0, 0, 0, 0 }, /* 147 */
    { 0, 0, 0, 0 }, /* 148 */
    { 0, 0, 0, 0 }, /* 149 */
    { 0, 0, 0, 0 }, /* 150 */
    { 0, 0, 0, 0 }, /* 151 */
    { 0, 0, 0, 0 }, /* 152 */
    { 0, 0, 0, 0 }, /* 153 */
    { 0, 0, 0, 0 }, /* 154 */
    { 0, 0, 0, 0 }, /* 155 */
    { 0, 0, 0, 0 }, /* 156 */
    { 0, 0, 0, 0 }, /* 157 */
    { 0, 0, 0, 0 }, /* 158 */
    { 0, 0, 0, 0 }, /* 159 */
    { 0, 0, 0, 0 }, /* 160 */
    { 0, 0, 0, 0 }, /* 161 */
    { 0, 0, 0, 0 }, /* 162 */
    { 0, 0, 0, 0 }, /* 163 */
    { 0, 0, 0, 0 }, /* 164 */
    { 0, 0, 0, 0 }, /* 165 */
    { 0, 0, 0, 0 }, /* 166 */
    { 0, 0, 0, 0 }, /* 167 */
    { 0, 0, 0, 0 }, /* 168 */
    { 0, 0, 0, 0 }, /* 169 */
    { 0, 0, 0, 0 }, /* 170 */
    { 0, 0, 0, 0 }, /* 171 */
    { 0, 0, 0, 0 }, /* 172 */
    { 0, 0, 0, 0 }, /* 173 */
    { 0, 0, 0, 0 }, /* 174 */
    { 0, 0, 0, 0 }, /* 175 */
    { 0, 0, 0, 0 }, /* 176 */
    { 0, 0, 0, 0 }, /* 177 */
    { 0, 0, 0, 0 }, /* 178 */
    { 0, 0, 0, 0 }, /* 179 */
    { 0, 0, 0, 0 }, /* 180 */
    { 0, 0, 0, 0 }, /* 181 */
    { 0, 0, 0, 0 }, /* 182 */
    { 0, 0, 0, 0 }, /* 183 */
    { 0, 0, 0, 0 }, /* 184 */
    { 0, 0, 0, 0 }, /* 185 */
    { 0, 0, 0, 0 }, /* 186 */
    { 0, 0, 0, 0 }, /* 187 */
    { 0, 0, 0, 0 }, /* 188 */
    { 0, 0, 0, 0 }, /* 189 */
    { 0, 0, 0, 0 }, /* 190 */
    { 0, 0, 0, 0 }, /* 191 */
    { 0, 0, 0, 0 }, /* 192 */
    { 0, 0, 0, 0 }, /* 193 */
    { 0, 0, 0, 0 }, /* 194 */
    { 0, 0, 0, 0 }, /* 195 */
    { 0, 0, 0, 0 }, /* 196 */
    { 0, 0, 0, 0 }, /* 197 */
    { 0, 0, 0, 0 }, /* 198 */
    { 0, 0, 0, 0 }, /* 199 */
    { 0, 0, 0, 0 }, /* 200 */
    { 0, 0, 0, 0 }, /* 201 */
    { 0, 0, 0, 0 }, /* 202 */
    { 0, 0, 0, 0 }, /* 203 */
    { 0, 0, 0, 0 }, /* 204 */
    { 0, 0, 0, 0 }, /* 205 */
    { 0, 0, 0, 0 }, /* 206 */
    { 0, 0, 0, 0 }, /* 207 */
    { 0, 0, 0, 0 }, /* 208 */
    { 0, 0, 0, 0 }, /* 209 */
    { 0, 0, 0, 0 }, /* 210 */
    { 0, 0, 0, 0 }, /* 211 */
    { 0, 0, 0, 0 }, /* 212 */
    { 0, 0, 0, 0 }, /* 213 */
    { 0, 0, 0, 0 }, /* 214 */
    { 0, 0, 0, 0 }, /* 215 */
    { 0, 0, 0, 0 }, /* 216 */
    { 0, 0, 0, 0 }, /* 217 */
    { 0, 0, 0, 0 }, /* 218 */
    { 0, 0, 0, 0 }, /* 219 */
    { 0, 0, 0, 0 }, /* 220 */
    { 0, 0, 0, 0 }, /* 221 */
    { 0, 0, 0, 0 }, /* 222 */
    { 0, 0, 0, 0 }, /* 223 */
    { 0, 0, 0, 0 }, /* 224 */
    { 0, 0, 0, 0 }, /* 225 */
    { 0, 0, 0, 0 }, /* 226 */
    { 0, 0, 0, 0 }, /* 227 */
    { 0, 0, 0, 0 }, /* 228 */
    { 0, 0, 0, 0 }, /* 229 */
    { 0, 0, 0, 0 }, /* 230 */
    { 0, 0, 0, 0 }, /* 231 */
    { 0, 0, 0, 0 }, /* 232 */
    { 0, 0, 0, 0 }, /* 233 */
    { 0, 0, 0, 0 }, /* 234 */
    { 0, 0, 0, 0 }, /* 235 */
    { 0, 0, 0, 0 }, /* 236 */
    { 0, 0, 0, 0 }, /* 237 */
    { 0, 0, 0, 0 }, /* 238 */
    { 0, 0, 0, 0 }, /* 239 */
    { 0, 0, 0, 0 }, /* 240 */
    { 0, 0, 0, 0 }, /* 241 */
    { 0, 0, 0, 0 }, /* 242 */
    { 0, 0, 0, 0 }, /* 243 */
    { 0, 0, 0, 0 }, /* 244 */
    { 0, 0, 0, 0 }, /* 245 */
    { 0, 0, 0, 0 }, /* 246 */
    { 0, 0, 0, 0 }, /* 247 */
    { 0, 0, 0, 0 }, /* 248 */
    { 0, 0, 0, 0 }, /* 249 */
    { 0, 0, 0, 0 }, /* 250 */
    { 0, 0, 0, 0 }, /* 251 */
    { 0, 0, 0, 0 }, /* 252 */
    { 0, 0, 0, 0 }, /* 253 */
    { 0, 0, 0, 0 }, /* 254 */
    { 0, 0, 0, 0 } /* 255 */
};
