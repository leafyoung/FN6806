## github - replit

rm main main.o main-debug
mkdir 22-operator
mv *.h *.cpp 22-operator/
git config pull.rebase true
git init
git remote add origin https://github.com/leafyoung/FN6806.git
git sparse-checkout init --cone
git sparse-checkout set 22-operator
git fetch
git pull origin main

git add 22-operator
git commit -m "22-operator"
git rebase origin/main
git push -u origin main
