@if exist personal.props goto NEXT
@copy tools\personal.props personal.props
:NEXT
@if exist theheaven.vcxproj.user goto END
@copy tools\theheaven.vcxproj.user theheaven.vcxproj.user
:END
