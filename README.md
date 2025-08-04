# Xenos

[![Build status](https://ci.appveyor.com/api/projects/status/eu6lpbla89gjgy5m?svg=true)](https://ci.appveyor.com/project/DarthTon/xenos)

> ⚙️ Advanced DLL injector for Windows — powered by the [Blackbone Library](https://github.com/DarthTon/Blackbone).

---

## ✨ Features

- ✅ Supports **x86** and **x64** processes and modules  
- 🧠 **Kernel-mode injection** (requires driver)  
- 🧱 **Manual mapping** of DLLs and kernel drivers  
- 🧼 Injection of **pure managed images** (no proxy DLL needed)  
- 🔄 **Cross-session** and **cross-desktop** injection (Windows 7)  
- ⚙️ Injection into **native processes** (e.g., ntdll-only)  
- 📦 **Custom initialization routine** support  
- 🕳️ Unlink modules post-injection  
- 🧬 **Thread hijacking** injection method  
- 🧩 Inject **x64 DLLs into WOW64** processes  
- 🧪 **Injection profiles** for quick reuse

---

## 🧰 Manual Map Highlights

- 🧭 Full relocation, import, delay import, bound import support  
- 🫥 Hide allocated image memory *(requires driver)*  
- 🔐 Static TLS, TLS callbacks, and security cookies  
- 🏷️ Image manifests & Side-by-Side (SxS) support  
- 🔍 Makes module visible to `GetModuleHandle`, `GetProcAddress`, etc.  
- 💣 Support for exceptions in private memory (DEP)  
- 💡 C++/CLI image support (*Use "Add loader reference"*)

---

## 🖥️ Supported OS

- Windows 7 through Windows 10 Windows 11 (x64 only)

---

## 📜 License

This project is licensed under the **MIT License**.  
All dependencies are under their respective licenses.

---

### 📂 Repository Info

- Main Developer: [DarthTon](https://github.com/DarthTon)  
- Base Library: [Blackbone](https://github.com/DarthTon/Blackbone)

---

