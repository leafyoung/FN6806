## github - replit

```
rm main main.o main-debug
mkdir 80-hash
mv *.h *.cpp 80-hash/
git config pull.rebase true
git init
git remote add origin https://github.com/leafyoung/FN6806.git
git sparse-checkout init --cone
git sparse-checkout set 80-hash
git fetch
git pull origin main

git add 80-hash
git commit -m "80-hash"
git rebase origin/main
git push -u origin main
```
