# Licenses and free/open software

## Defining free software

According to the [definition of free software][free_software_definition] provided by the [Free Software Foundation][free_software_foundation], the word "free" is a matter of liberty, not price[^1].

A "free software" is such if the program's users have the four essential freedoms:

1. The freedom to run the program as you wish, for any purpose.
2. The freedom to study how the program works, and change it so it does your computing as you wish. Access to the source code is a precondition for this.
3. The freedom to redistribute copies so you can help others.
4. The freedom to distribute copies of your modified versions to others.

[^1]: In Spanish, "free software" is translated as "software libre", not "software gratis".

## Defining open source software

Instead of emphasizing the users' freedom, the [Open Source Software definition] [open_software_definition] focuses on the licensing of the software and its availability.

It is a requirement to provide the source code and executable binaries, and to allow free redistribution. However, open source does not bother to specify anything about the software's modified versions. Therefore, the following things might happen:

1. An open source software can restrict its users from making modified versions.
2. *Tivoization*: Many products check signatures on their executable programs to block users from effectively using different executables (so called "secure boot"); only one privileged company can make executables that can run in the device an use its full capabilities. Even if the executable is made from free source code, the users cannot usefully run modified versions of it, so the executable is de-facto non-free.

## Licenses

I am not a lawyer. [This page][licenses_tldr] has a good summary of the licenses. [This page][choose_license] helps you choose the right license for your project.

In the following sections first we will discuss the requirements to use the licenses. Then, the most important aspects of the most common ones.

### Requirements

You should include both a copy of the license in the repository, as well as a copyright notice[^2] in each source file ([REF][source_file_license]).

[^2]: The license itself should provide you with the copyright notice to copy-paste in each file.

### GPL-v2

Used by Linux. Enforces the four essential freedoms. Only applies to the changes in the Kernel itself. You can use Linux in a closed source application. If you modify it in any way, you need to provide the changes.

### GPL-v3

Used by the GNU project. The GPL-v3 was born in response to the *Tivoization* issue. It is a *copyleft* license, meaning that derivate work that uses or modifies the licensed product must use the same license.

Therefore, if a project uses a software under the GPL-v3, all the project must adhere to the GPL-v3 license and therefore be "free software".

There is the exception of the LGPL-v3, which is used for libraries. The whole project doesn't need to be GPL-v3 compliant if linked against a library with LGPL-v3 license.

### Apache 2.0

Changes made to the licensed material must be documented. It does not grant trademark rights.

### MIT

This is the most permissive license. The only requirement being that the copyright and the license must be included on usage and in derivative works.

<!-- External links -->
[free_software_definition]: https://www.gnu.org/philosophy/free-sw.en.html
[open_software_definition]: https://opensource.org/osd
[free_software_foundation]: https://www.fsf.org/
[licenses_tldr]: https://www.tldrlegal.com/
[source_file_license]: https://www.gnu.org/licenses/gpl-faq.en.html#NoticeInSourceFile
[choose_license]: https://choosealicense.com/licenses/
