## [0.5.0](https://github.com/B-AJ-Amar/my-kernel/compare/v0.4.0...v0.5.0) (2026-09-20)

### Features

* add ctrl +c interupt ([144fbe4](https://github.com/B-AJ-Amar/my-kernel/commit/144fbe43a0b08f6e190b9f1b417f24c8537c68b4))

## [0.4.0](https://github.com/B-AJ-Amar/my-kernel/compare/v0.3.0...v0.4.0) (2026-09-20)

### Features

* add editor ([ba70f54](https://github.com/B-AJ-Amar/my-kernel/commit/ba70f54341294b68b4abc15ee3c8c41bb55e5b84))
* add in memory file system ([9912402](https://github.com/B-AJ-Amar/my-kernel/commit/9912402ea79ecd5bd4ddd22ad841f326aac537b5))
* add shell ([7656615](https://github.com/B-AJ-Amar/my-kernel/commit/765661592b4fb2c397bbd95e1d78b962e8f1bb0b))
* **shell:** add cli ([8c2574f](https://github.com/B-AJ-Amar/my-kernel/commit/8c2574fafc577eeff4c520cb93b01ae1424afc85))
* **shell:** add cmds: print, info, uptime, rename,move ([2a7f347](https://github.com/B-AJ-Amar/my-kernel/commit/2a7f347c3bb189b6483b26f3789052d01fa264cb))

### Bug Fixes

* **boot:** disk read problem ([3edfce3](https://github.com/B-AJ-Amar/my-kernel/commit/3edfce378690570fd395dde3b99388ab1b09deca))
* page fault ([5f308bc](https://github.com/B-AJ-Amar/my-kernel/commit/5f308bca8ddaff71cba63a840560f720c18267ef))

### Refactoring

* tty ([978e188](https://github.com/B-AJ-Amar/my-kernel/commit/978e188d4f19ab2a03913965def0b9729765f078))
* update filesystem initialization and command registration ([06d9963](https://github.com/B-AJ-Amar/my-kernel/commit/06d9963e9f011875358d7a10d4918851c4a3e028))

## [0.3.0](https://github.com/B-AJ-Amar/my-kernel/compare/v0.2.0...v0.3.0) (2026-09-12)

### Features

* add basic scheduling functions ([15c4260](https://github.com/B-AJ-Amar/my-kernel/commit/15c4260d1aa8bce764c0f9c3ba8abf3020ed70fd))
* add scheduler header ([3a906a3](https://github.com/B-AJ-Amar/my-kernel/commit/3a906a3933a673399d65cd2e0c844997bbdc8a2a))
* add sheduler ([b1909b1](https://github.com/B-AJ-Amar/my-kernel/commit/b1909b1c6e4bf21d7508a140f524ec28d8b66572))
* **shed:** add contex switch handling ([b96030e](https://github.com/B-AJ-Amar/my-kernel/commit/b96030ecf3e6ac061d08898f5d6895cec73f42dc))

### Bug Fixes

* 1/2 sheduler ([e7022c4](https://github.com/B-AJ-Amar/my-kernel/commit/e7022c46317ea430619abc6aa664833fd6e4cc26))
* mem allocation ([4077de4](https://github.com/B-AJ-Amar/my-kernel/commit/4077de43542864dd08ffcf5e60e997f373748347))
* memory init alloc ([632f8fe](https://github.com/B-AJ-Amar/my-kernel/commit/632f8fe62367a7d52148aa56e7302491fff6cb60))
* quick ix for protected mode stack pointer ([a83157e](https://github.com/B-AJ-Amar/my-kernel/commit/a83157e1d944456cf58cd499fa3c8eba1dc937fd))

### Refactoring

* addr datatypes ([170acd0](https://github.com/B-AJ-Amar/my-kernel/commit/170acd0442b48a68d9b5dc3031c1facd6ce762dc))
* change sp to safer address ([dc1eb24](https://github.com/B-AJ-Amar/my-kernel/commit/dc1eb24983fff9329343dc3ae884a2ca9f340339))
* some changes ([7106126](https://github.com/B-AJ-Amar/my-kernel/commit/71061264e4cd1137e8965ee42c4524b7b6d6425f))

### Internal

* foramt ([1b1c3d0](https://github.com/B-AJ-Amar/my-kernel/commit/1b1c3d03a7f5a49a4203139b374d53654fb8cb60))

## [0.2.0](https://github.com/B-AJ-Amar/my-kernel/compare/v0.1.0...v0.2.0) (2026-08-21)

### Features

* add clear screen function ([6df9d98](https://github.com/B-AJ-Amar/my-kernel/commit/6df9d98187d529079474f106fa46b2f98b359a32))
* add vmm h ([c9a64e2](https://github.com/B-AJ-Amar/my-kernel/commit/c9a64e2d1f61bfbfa2f9719bae45a9ffac2eaf18))
* add vmm init ([f118bfe](https://github.com/B-AJ-Amar/my-kernel/commit/f118bfe71830f6cdd938f4ea974670a38f5d6557))
* **memory:** add physical memory handling ([5b8a6b5](https://github.com/B-AJ-Amar/my-kernel/commit/5b8a6b509db2c8850402c5531b88398136f5423b))
* **memory:** implement heap management functions and memory initialization ([d12e3aa](https://github.com/B-AJ-Amar/my-kernel/commit/d12e3aa7d57a9ddbdf23d19bde4c33c2649c1894))
* **memory:** implement memory management functions ([a44d586](https://github.com/B-AJ-Amar/my-kernel/commit/a44d5863931984f17261c2ab789ab4b87a30eae3))
* **pmm:** add asm memor ([3893432](https://github.com/B-AJ-Amar/my-kernel/commit/3893432cbf121a9c23dc505f03526aee29f91ff7))
* **print:** add %llx  support ([71dc55e](https://github.com/B-AJ-Amar/my-kernel/commit/71dc55e1cc47e5c288e90f7c952f86e157996d6e))
* send boot infos to the kernel ([a63127d](https://github.com/B-AJ-Amar/my-kernel/commit/a63127d3ced2a8ce28054b5b317087afb1d46f47))

### Bug Fixes

* calculate kernel real size + fix vmm ([7ff22ee](https://github.com/B-AJ-Amar/my-kernel/commit/7ff22ee54752a1fc19219a8645fecae0e149de4a))
* include string.h for memset usage in heap.c ([6bb1990](https://github.com/B-AJ-Amar/my-kernel/commit/6bb1990a2ef50dda426b3e1b6d5900005cfbe8c7))
* memory detection ([c3115ca](https://github.com/B-AJ-Amar/my-kernel/commit/c3115cac7d7c5a464ca3bda9abc8a1050f28df33))

### Refactoring

* file structure ([adad8f0](https://github.com/B-AJ-Amar/my-kernel/commit/adad8f0473eef9112e292fa07398281f651791ba))
* **files:** memory/ -> pmm/ ([0825408](https://github.com/B-AJ-Amar/my-kernel/commit/082540820efbb7576c338e333d6d3c946ec73873))
* inhance kernel size detection ([d38cd64](https://github.com/B-AJ-Amar/my-kernel/commit/d38cd64a1fb71e4427b6c90c89554d987396c3d8))
* simplify code strucuture + remove the usless scripts ([56535c3](https://github.com/B-AJ-Amar/my-kernel/commit/56535c32838f8f6594730d2efb1737979e9e928f))
* **timer:** 32 ->64 ([2b1128c](https://github.com/B-AJ-Amar/my-kernel/commit/2b1128cd996f5cfd5690ea7ede735de13abcb474))

### Internal

* fmt ([7dc9eb0](https://github.com/B-AJ-Amar/my-kernel/commit/7dc9eb0d4664ca71911963c07bdfce2db10a8ce5))
* fmt ([6864f25](https://github.com/B-AJ-Amar/my-kernel/commit/6864f25287a4cfd6f2b888950e6c99250a4801e0))
* fmt ([15737b5](https://github.com/B-AJ-Amar/my-kernel/commit/15737b5debfb91127e436d786b1de8e044a5c2c2))
* update readme ([8080dbc](https://github.com/B-AJ-Amar/my-kernel/commit/8080dbc286a0b8d10af68c848d6a773cc3ab4825))

## [0.1.0](https://github.com/B-AJ-Amar/my-kernel/compare/v0.0.0...v0.1.0) (2026-08-03)

### Features

*  add sleep function ([6369c92](https://github.com/B-AJ-Amar/my-kernel/commit/6369c9283ed41ab74d12e7ebe45faa3ea9078512))
* **timer:** add pit driver ([09b56d4](https://github.com/B-AJ-Amar/my-kernel/commit/09b56d4c4348c08e2993344f4410a40e1c368151))

## 0.0.0 (2026-07-28)

### Features

* add basic stdio ([51eb79e](https://github.com/B-AJ-Amar/my-kernel/commit/51eb79e6990e2ed0b35504e7dcbd1f34dfe1a9ab))
* add console ([0538a25](https://github.com/B-AJ-Amar/my-kernel/commit/0538a25dd5ec96dd4b49bbbe7a5829cf3d660ba4))
* add keyboard driver ([55562bf](https://github.com/B-AJ-Amar/my-kernel/commit/55562bf947b76fd70835401f432c2558f05264be))
* add ps/2driver ([5c439be](https://github.com/B-AJ-Amar/my-kernel/commit/5c439bee73e99e0903a1b8047255da7aa752111b))
* add stdarg header for variable argument handling ([b61d6b1](https://github.com/B-AJ-Amar/my-kernel/commit/b61d6b1ccb58e5353b560bb9b70ef3f0950a7e2a))
* add string + stddef libs ([62d84c9](https://github.com/B-AJ-Amar/my-kernel/commit/62d84c9489ba51542e70548732760a94a92a87e5))
* add tty ([edd67d1](https://github.com/B-AJ-Amar/my-kernel/commit/edd67d10bfc191ef37684b3cd580811675d597ae))
* add vga cursor handling + fix overflow problem ([5dfee63](https://github.com/B-AJ-Amar/my-kernel/commit/5dfee630888e1cbc36d2f9f306a22e8ebdc58b56))
* basic bootloader ([7320c33](https://github.com/B-AJ-Amar/my-kernel/commit/7320c33ae7312fcd79013e1da3b730fc5170effd))
* **boot:** load kernel ([0e7c4e1](https://github.com/B-AJ-Amar/my-kernel/commit/0e7c4e12aa2ff3052267a903b13d3a35aea8908f))
* **console:** add color support ([3a3ddfb](https://github.com/B-AJ-Amar/my-kernel/commit/3a3ddfbbf2a6f4be0f7b45716cfb58644713e472))
* **console:** rename console variable to active_console for clarity ([ff40add](https://github.com/B-AJ-Amar/my-kernel/commit/ff40addbc2253f5d45568e9634fd7b3732dc6acd))
* enable protected mode ([4d9d15a](https://github.com/B-AJ-Amar/my-kernel/commit/4d9d15a62fc6b35723881e229fd28168bcba8a3d))
* **idt:** implement interrupt descriptor table and handlers ([2cdc552](https://github.com/B-AJ-Amar/my-kernel/commit/2cdc5520e3b958512b0f2aa0546c2a81965476c0))
* **interupt:** add hardware interupts ([e5124d0](https://github.com/B-AJ-Amar/my-kernel/commit/e5124d028214e1a29888d8c1d2cabffd317155c4))
* **irq:** implement IRQ handling and PIC initialization ([3ae2ca9](https://github.com/B-AJ-Amar/my-kernel/commit/3ae2ca9967092b861b1e45338499fc56a60e3ed7))
* **keyboard:** enhance keyboard driver with event handling and state management ([b4d67d9](https://github.com/B-AJ-Amar/my-kernel/commit/b4d67d945f06a6e421f7366efc10cec9a55ada7c))
* **tty:** add tty header ([59e5116](https://github.com/B-AJ-Amar/my-kernel/commit/59e51166cc0e2923384e7dd4e83fa5870f59d0e5))
* update kernel size handling in boot.asm and script ([fd83c82](https://github.com/B-AJ-Amar/my-kernel/commit/fd83c82358b50f412d615969176687f21a2cebfa))
* **vga:** add scroll ([79e8006](https://github.com/B-AJ-Amar/my-kernel/commit/79e8006b8db4f29b01bf686b4ea6677228047f45))
* **vga:** add VGA driver with color support ([ac4aa3c](https://github.com/B-AJ-Amar/my-kernel/commit/ac4aa3c580be1b61e42a1206357b7d04c820787d))
* **vga:** enhance character handling in vga_putchar function ([edc0f54](https://github.com/B-AJ-Amar/my-kernel/commit/edc0f5402380f605f9bbc0c511d599caeb4f0890))

### Bug Fixes

* kernel size move ([a5bdd5e](https://github.com/B-AJ-Amar/my-kernel/commit/a5bdd5ece2daa93bb1d0a6790a8a3b96aa150edc))
* pic init ([eab88f6](https://github.com/B-AJ-Amar/my-kernel/commit/eab88f6da70c8589a7f916ce83744efd5e3cba6a))
* **vga:** backspace logic ([ad5c172](https://github.com/B-AJ-Amar/my-kernel/commit/ad5c17205ca388ba248aa36a749f01f1b4abd8a8))

### Refactoring

* change boot file path ([b2cce2a](https://github.com/B-AJ-Amar/my-kernel/commit/b2cce2aff604a7fc30da2c8c8539d7458360b6db))
* file structure (add arch/ folder) ([739c712](https://github.com/B-AJ-Amar/my-kernel/commit/739c712e432e56846623aa52b89cd54ce46e0fe7))
* **keyboard:** simplify event queue functions and improve event handling ([5ab2f97](https://github.com/B-AJ-Amar/my-kernel/commit/5ab2f97a0b63d7b2f9e2f61d44466063ba2f7e38))
* **vga:** change cursor ([da2a000](https://github.com/B-AJ-Amar/my-kernel/commit/da2a00092296306083661c53fcd7cb26b8f625c1))

### CI/CD

* add ci/cd workflows ([6478ce1](https://github.com/B-AJ-Amar/my-kernel/commit/6478ce150d99e422abc222fc33aa6ed43e381a58))
* add pre-commit ([1acee69](https://github.com/B-AJ-Amar/my-kernel/commit/1acee69821018a73891b442614b22974c2fda2d8))
* fix ci pipline ([ff4bd5b](https://github.com/B-AJ-Amar/my-kernel/commit/ff4bd5be5d754e9d3b2aec76cbfe4de89204329f))
* fix kernel-builder image ([9c6acf1](https://github.com/B-AJ-Amar/my-kernel/commit/9c6acf10cb734c825c985bdae9b717ebf06b49c8))
* **fix:** update the package.json ([f137e0e](https://github.com/B-AJ-Amar/my-kernel/commit/f137e0e7d4e1ab1e05c8689d5d3c4584d430aa70))
* update release trigger ([7022c2f](https://github.com/B-AJ-Amar/my-kernel/commit/7022c2f65387dd9596adf17b64e4c0f712328b36))
* update test workflow ([e073b24](https://github.com/B-AJ-Amar/my-kernel/commit/e073b24a8cbb28b4274a5d425dc5a07bfb8fee24))

### Internal

* add build scripts ([08f607f](https://github.com/B-AJ-Amar/my-kernel/commit/08f607ff96953a611d528842f03c438520aa3170))
* add debug setup ([88e9d82](https://github.com/B-AJ-Amar/my-kernel/commit/88e9d82f0eb9e8f6e25ec6b0d355cf107c1cc30e))
* add makefile and gitignore ([410e393](https://github.com/B-AJ-Amar/my-kernel/commit/410e393920393ea4f23b80c4ffca50d6e418c487))
* add releaser config ([71024bd](https://github.com/B-AJ-Amar/my-kernel/commit/71024bd2c0de34c59074a8f5f2d34e19f8fd0f82))
* fmt ([2f48728](https://github.com/B-AJ-Amar/my-kernel/commit/2f48728b18b556d9871a8c66763650ded9f47f46))
* format ([99b7cb4](https://github.com/B-AJ-Amar/my-kernel/commit/99b7cb48a60fd4a5f767e7bdfa079ae09cefe49a))
* format ([f995af8](https://github.com/B-AJ-Amar/my-kernel/commit/f995af8dd6ccf24e40f5187bce8635066e9b01ef))
* format ([2fe1140](https://github.com/B-AJ-Amar/my-kernel/commit/2fe1140a1ac24249dc01ae723ea6fab0053281b3))
