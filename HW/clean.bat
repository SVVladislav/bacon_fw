SET NAME=bacon_v9\bacon_fw

rd bacon_v9\.Xil /s /q

rd %NAME%.cache /s /q
rd %NAME%.gen /s /q
rd %NAME%.hw /s /q
rd %NAME%.ioplanning /s /q
rd %NAME%.ip_user_files /s /q
rd %NAME%.runs /s /q
rd %NAME%.sim /s /q
rd %NAME%.tmp /s /q

del *.jou /s /q 
del *.log /s /q
del *.str /s /q
del *.dmp /s /q 
