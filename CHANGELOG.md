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
