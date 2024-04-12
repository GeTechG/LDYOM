import Layout from "@theme/Layout";
import {JSX} from "react";
import Beta0_8_0_Preview_10 from "@site/src/pages/download/versions/beta0_8_0_Preview_10";
import Beta0_8_0_Preview_9 from "@site/src/pages/download/versions/beta0_8_0_Preview_9";
import Beta_0_7_2 from "@site/src/pages/download/versions/beta_0_7_2";
import Beta_0_7_1 from "@site/src/pages/download/versions/beta_0_7_1";
import Beta_0_7_0 from "@site/src/pages/download/versions/beta_0_7_0";


export default function Downloads(): JSX.Element {
    return (
        <Layout
            description="Create your world">
            <div>
                <Beta0_8_0_Preview_10/>
                <Beta0_8_0_Preview_9/>
                <Beta_0_7_2/>
                <Beta_0_7_1/>
                <Beta_0_7_0/>
            </div>
        </Layout>
    );
}