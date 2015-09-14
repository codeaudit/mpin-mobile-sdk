package com.certivox.view;


import java.util.ArrayList;
import java.util.List;

import com.certivox.mpin.R;

import android.content.Context;
import android.content.res.Resources;
import android.content.res.TypedArray;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.drawable.ShapeDrawable;
import android.graphics.drawable.shapes.OvalShape;
import android.util.AttributeSet;
import android.view.Gravity;
import android.view.View;


public class SelectionCircles extends View {

    public enum CircleStyle {
        FILLED(0), HOLLOW(1);

        public int value;


        private CircleStyle(int value) {
            this.value = value;
        }


        public static CircleStyle fromInt(int value) {
            switch (value) {
            case 1:
                return HOLLOW;
            case 0:
            default:
                return FILLED;
            }
        }
    }

    private static final String ANDROID_NAMESPACE      = "http://schemas.android.com/apk/res/android";
    private static final String GRAVITY_ATTRIBUTE_NAME = "gravity";

    private int           mCirclesCount;
    private float         mCircleDiameter;
    private float         mDistanceBetweenCircles;
    private int           mDefaultColor;
    private int           mSelectedColor;
    private ShapeDrawable mCircle;
    private List<Integer> mSelectedPositions;
    private CircleStyle   mSelectedStyle;
    private CircleStyle   mDefaultStyle;
    private int           mGravity;


    public SelectionCircles(Context context, AttributeSet attrs, int defStyle) {
        super(context, attrs, defStyle);
        init();
        applyAttrs(attrs, defStyle);
    }


    public SelectionCircles(Context context, AttributeSet attrs) {
        super(context, attrs);
        init();
        applyAttrs(attrs);
    }


    public SelectionCircles(Context context) {
        super(context);
        init();
    }


    public int getCount() {
        return mCirclesCount;
    }


    public void setCount(int circlesCount) {
        if (circlesCount <= 0) {
            mCirclesCount = 0;
        } else {
            mCirclesCount = circlesCount;
        }
        invalidate();
        requestLayout();
    }


    public List<Integer> getSelectedPositions() {
        return mSelectedPositions;
    }


    public void setSelectedPositions(int... selectedDotPositions) {
        mSelectedPositions.clear();
        if (selectedDotPositions != null && selectedDotPositions.length > 0) {
            for (int selectedPos : selectedDotPositions) {
                if (selectedPos >= 0 && selectedPos < mCirclesCount) {
                    mSelectedPositions.add(selectedPos);
                }
            }
        }
        invalidate();
        requestLayout();
    }


    public void selectPosition(int position) {
        if (position >= 0 && position < mCirclesCount) {
            mSelectedPositions.add(position);
            invalidate();
            requestLayout();
        }
    }


    public void selectAll() {
        mSelectedPositions.clear();
        for (int i = 0; i < mCirclesCount; i++) {
            mSelectedPositions.add(i);
        }
        invalidate();
        requestLayout();
    }


    public void deselectAll() {
        mSelectedPositions.clear();
        invalidate();
        requestLayout();
    }


    public void deselectPosition(int position) {
        if (mSelectedPositions.remove(Integer.valueOf(position))) {
            invalidate();
            requestLayout();
        }
    }


    public float getCircleDiameter() {
        return mCircleDiameter;
    }


    public void setCircleDiameter(float circleDiameter) {
        if (circleDiameter > 0 && mCircleDiameter != circleDiameter) {
            mCircleDiameter = circleDiameter;
            invalidate();
            requestLayout();
        }
    }


    public float getDistanceBetweenCircles() {
        return mDistanceBetweenCircles;
    }


    public void setDistanceBetweenCircles(float distance) {
        if (distance > 0 && distance != mDistanceBetweenCircles) {
            mDistanceBetweenCircles = distance;
            invalidate();
            requestLayout();
        }
    }


    public int getDefaultColor() {
        return mDefaultColor;
    }


    public void setDefaultColor(int defaultColor) {
        if (defaultColor != mDefaultColor) {
            mDefaultColor = defaultColor;
            invalidate();
            requestLayout();
        }
    }


    public int getSelectedColor() {
        return mSelectedColor;
    }


    public void setSelectedColor(int selectedColor) {
        if (selectedColor != mSelectedColor) {
            mSelectedColor = selectedColor;
            invalidate();
            requestLayout();
        }
    }


    public void setSelectedStyle(CircleStyle selectedStyle) {
        if (selectedStyle != null && selectedStyle != mSelectedStyle) {
            mSelectedStyle = selectedStyle;
            invalidate();
            requestLayout();
        }
    }


    public void setDefaultStyle(CircleStyle defaultStyle) {
        if (defaultStyle != null && defaultStyle != mDefaultStyle) {
            mDefaultStyle = defaultStyle;
            invalidate();
            requestLayout();
        }
    }


    public CircleStyle getDefaultStyle() {
        return mDefaultStyle;
    }


    public CircleStyle getSelectedStyle() {
        return mSelectedStyle;
    }


    @Override
    protected void onMeasure(int widthMeasureSpec, int heightMeasureSpec) {
        int width = MeasureSpec.getSize(widthMeasureSpec);
        int widthMode = MeasureSpec.getMode(widthMeasureSpec);
        int height = MeasureSpec.getSize(heightMeasureSpec);
        int heightMode = MeasureSpec.getMode(heightMeasureSpec);

        int pointsWidth = getContentWidth();
        int pointsHeight = getContentHeight();

        switch (widthMode) {
        case MeasureSpec.UNSPECIFIED:
            widthMeasureSpec = MeasureSpec.makeMeasureSpec(pointsWidth, MeasureSpec.EXACTLY);
            break;
        case MeasureSpec.AT_MOST:
            if (pointsWidth < width) {
                widthMeasureSpec = MeasureSpec.makeMeasureSpec(pointsWidth, MeasureSpec.EXACTLY);
                break;
            }
        case MeasureSpec.EXACTLY:
        default:
            // When the provided space is fixed or less than the desired points size - use the provided space
            break;
        }

        switch (heightMode) {
        case MeasureSpec.UNSPECIFIED:
            heightMeasureSpec = MeasureSpec.makeMeasureSpec(pointsHeight, MeasureSpec.EXACTLY);
            break;
        case MeasureSpec.AT_MOST:
            if (pointsHeight < height) {
                heightMeasureSpec = MeasureSpec.makeMeasureSpec(pointsHeight, MeasureSpec.EXACTLY);
                break;
            }
        case MeasureSpec.EXACTLY:
        default:
            // When the provided space is fixed or less than the desired points size - use the provided space
            break;
        }

        super.onMeasure(widthMeasureSpec, heightMeasureSpec);
    }


    @Override
    protected void onDraw(Canvas canvas) {
        super.onDraw(canvas);
        Paint dotPaint = mCircle.getPaint();
        int horizontalPadding = 0;
        int verticalPadding = 0;
        if (mGravity == Gravity.CENTER) {
            horizontalPadding = (getWidth() - getContentWidth()) / 2;
            verticalPadding = (getHeight() - getContentHeight()) / 2;
        }

        for (int i = 0; i < mCirclesCount; i++) {
            if (mSelectedPositions.contains(i)) {
                dotPaint.setColor(mSelectedColor);
                applyStyle(dotPaint, mSelectedStyle);
            } else {
                dotPaint.setColor(mDefaultColor);
                applyStyle(dotPaint, mDefaultStyle);
            }
            int left = (int) (i * mCircleDiameter + i * mDistanceBetweenCircles);
            int right = (int) (left + mCircleDiameter);
            mCircle.setBounds(left + horizontalPadding, verticalPadding, right + horizontalPadding,
                    (int) mCircleDiameter + verticalPadding);
            mCircle.draw(canvas);
        }
    }


    private void init() {
        mSelectedPositions = new ArrayList<Integer>();
        mCircle = new ShapeDrawable(new OvalShape());
        Resources res = getContext().getResources();
        mCircleDiameter = res.getDimension(R.dimen.scDefaultCircleDiameter);
        mDistanceBetweenCircles = res.getDimension(R.dimen.scDefaultDistanceBetweenCircles);
        mDefaultColor = mSelectedColor = Color.BLACK;
        mCirclesCount = 0;
        mDefaultStyle = CircleStyle.HOLLOW;
        mSelectedStyle = CircleStyle.FILLED;
        mGravity = Gravity.NO_GRAVITY;
        setWillNotDraw(false);
    }


    private void applyAttrs(AttributeSet attrs) {
        applyAttrs(attrs, 0);
    }


    private void applyAttrs(AttributeSet attrs, int defStyle) {
        TypedArray attributesTA = getContext().getTheme().obtainStyledAttributes(attrs, R.styleable.SelectionCircles, 0,
                defStyle);

        try {
            mDefaultColor = attributesTA.getColor(R.styleable.SelectionCircles_defaultColor, mDefaultColor);
            mSelectedColor = attributesTA.getColor(R.styleable.SelectionCircles_selectedColor, mSelectedColor);
            mCircleDiameter = attributesTA.getDimension(R.styleable.SelectionCircles_circleDiameter, mCircleDiameter);
            mDistanceBetweenCircles = attributesTA.getDimension(R.styleable.SelectionCircles_spaceBetweenCircles,
                    mDistanceBetweenCircles);
            mCirclesCount = attributesTA.getInteger(R.styleable.SelectionCircles_circlesCount, mCirclesCount);
            mSelectedStyle = CircleStyle
                    .fromInt(attributesTA.getInteger(R.styleable.SelectionCircles_selectedStyle, mSelectedStyle.value));
            mDefaultStyle = CircleStyle
                    .fromInt(attributesTA.getInteger(R.styleable.SelectionCircles_defaultStyle, mDefaultStyle.value));
            mGravity = attrs.getAttributeIntValue(ANDROID_NAMESPACE, GRAVITY_ATTRIBUTE_NAME, mGravity);
        } finally {
            attributesTA.recycle();
        }

    }


    private void applyStyle(Paint paint, CircleStyle style) {
        switch (style) {
        case HOLLOW:
            paint.setStyle(Paint.Style.STROKE);
            break;
        case FILLED:
        default:
            paint.setStyle(Paint.Style.FILL);
            break;
        }
    }


    private int getContentWidth() {
        return (int) ((mCirclesCount * mCircleDiameter) + (mCirclesCount - 1) * mDistanceBetweenCircles);
    }


    private int getContentHeight() {
        return (int) mCircleDiameter;
    }
}
