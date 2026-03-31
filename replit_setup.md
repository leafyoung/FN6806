# github - replit

```bash
rm main main.o main-debug
mkdir -p 81-concept
mv *.h *.cpp 81-concept/
git config pull.rebase true
git init
git remote add origin https://github.com/leafyoung/FN6806.git
git sparse-checkout init --cone
git sparse-checkout set 81-concept
git fetch
git pull origin main

git add 81-concept
git commit -m "81-concept"
git rebase origin/main
git push -u origin main
```
