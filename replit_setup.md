## github - replit

rm main main.o main-debug
mkdir 12-class_point
mv *.h *.cpp 12-class_point/
git config pull.rebase true
git init
git remote add origin https://github.com/leafyoung/FN6806.git
git sparse-checkout init --cone
git sparse-checkout set 12-class_point
git fetch
git pull origin main

git add 12-class_point
git commit -m "12-class_point"
git rebase origin/main
git push -u origin main
