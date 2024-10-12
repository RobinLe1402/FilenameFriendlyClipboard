# FilenameFriendlyClipboard

This repo contains the Windows application `FilenameFriendlyClipboard`
that, when called, (silently) changes the contents of the clipboard to
be usable as a filename under Microsoft Windows (if the current content
of the clipboard is text compatible).

This is achieved by substituting disallowed characters with similar
looking characters.

## Conversion table
| Original            | Replacement      |
|---------------------|------------------|
| `/`                 |  `／` (`U+FF0F`) |
| `\`                 |  `＼` (`U+FF3C`) |
| `:`                 |  `：` (`U+FF1A`)  |
| `*`                 |  `＊` (`U+FF0A`) |
| `?`                 |  `？` (`U+FF1F`) |
| `"`                 |  `''` (2x `'`)   |
| `<`                 |  `＜` (`U+FF1C`) |
| `>`                 |  `＞` (`U+FF1E`) |
| <code>&#124;</code> |  `｜` (`U+FF5C`) |


## Development history

This application was originally developed as part of my
[`tools`](https://rle.sh/git-repo/tools) repository. However, as time
moved on I decided to split that repository up into one repository for
each application it contained. And I actively decided against keeping
the file history because...

- all applications had a very short development time with few commits;
- all new repositories would have the commits of all other applications
  too;
- when I decided to split the repository, I also updated the source code
  to match my new default repository structure and to be more efficient;
- functionally, all applications were finished at that time, so I
  wouldn't have to look at previous revisions.

Hence, I decided to do a fresh start for all the tools.
